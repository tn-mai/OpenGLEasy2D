/**
* @file Command.cpp
*/
#include "Command.h"
#include "GLFWEW.h"
#include "Sprite.h"
#include "Font.h"
#include "Audio.h"
#include <glm/gtc/constants.hpp>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <iostream>
#include <stdarg.h>

bool hasQuitRequest = false;

namespace /* unnamed */ {

enum easing_type
{
  linear,
  ease_in,
  ease_out,
  ease_in_out,
  ease_out_back,
  ease_out_bounce,
};

template<typename T>
struct action
{
  T start;
  T end;
  easing_type easing;
  float seconds;

  float timer;

  void init(const T& s, const T& e, int ease, float sec) {
    start = s;
    end = e;
    easing = static_cast<easing_type>(glm::clamp(ease, 0, static_cast<int>(ease_out_bounce)));
    seconds = sec;
    timer = 0;
  }

  T update(float delta) {
    if (seconds <= 0) {
      return end;
    }
    timer = std::min(timer + delta, seconds);
    float ratio = (timer / seconds);
    switch (easing) {
    default:
    case linear: break;
    case ease_in: ratio *= ratio; break;
    case ease_out: ratio = 2.0f * ratio - ratio * ratio; break;
    case ease_in_out:
      ratio *= 2.0f;
      if (ratio < 1.0f) {
        ratio *= ratio;
      } else {
        ratio -= 1.0f;
        ratio = 2.0f * ratio - ratio * ratio;
        ratio += 1.0f;
      }
      ratio *= 0.5f;
      break;
    case ease_out_back: {
      static const float scale = 1.70158f * 1.525f;
      ratio -= 1;
      ratio = 1 + 2.70158f * ratio * ratio * ratio + 1.70158f * ratio * ratio;
      break;
    }
    case ease_out_bounce: {
      if (ratio < (1 / 2.75f)) {
        ratio = 7.5625f * ratio * ratio;
      } else if (ratio < (2 / 2.75f)) {
        ratio -= 1.5f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.75f;
      } else if (ratio < (2.5 / 2.75f)) {
        ratio -= 2.25f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.9375f;
      } else {
        ratio -= 2.625f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.984375f;
      }
      break;
    }
    }
    return start + (end - start) * ratio;
  }
};

using translate_action = action<glm::vec2>;
using scale_action = action<glm::vec2>;
using rotate_action = action<float>;
using color_action = action<glm::vec4>;

struct actable_sprite : Sprite
{
  translate_action translate;
  scale_action scale;
  rotate_action rotate;
  color_action color;

  void init_action() {
    translate.init(Position(), Position(), linear, 0);
    scale.init(Scale(), Scale(), linear, 0);
    rotate.init(Rotation(), Rotation(), linear, 0);
    color.init(Color(), Color(), linear, 0);
  }

  virtual void Update(glm::f32 delta) override {
    Sprite::Update(delta);
    Position(glm::vec3(translate.update(delta), 0));
    Scale(scale.update(delta));
    Rotation(rotate.update(delta));
    Color(color.update(delta));
  }
};

struct color_filter : Sprite
{
  action<glm::f32> action;

  virtual void Update(glm::f32 delta) override {
    Sprite::Update(delta);
    glm::vec4 c = Color();
    c.w = action.update(delta);
    Color(c);
  }
};

Sprite rootNode;
std::vector<actable_sprite> spriteBuffer;
SpriteRenderer spriteRenderer;

color_filter colorFilter;
SpriteRenderer colorFilterRenderer;

struct text_info
{
  glm::vec2 pos;
  std::wstring text;
};
std::vector<text_info> textList;
Font::Renderer fontRenderer;

std::mt19937 randomEngine;

std::string bgmFilename;
Audio::SoundPtr bgm;
float seVolume = 1.0f;
float bgmVolume = 1.0f;

} // unnamed namespace

/**
* sjis文字列からutf16文字列を得る.
*/
std::wstring sjis_to_utf16(const char* p)
{
  std::wstring tmp;
  tmp.resize(strlen(p) + 1);
  mbstowcs(&tmp[0], p, tmp.size());
  return tmp;
}

/**
* スクリーン座標に対応するクリップ座標を得る.
*/
glm::vec2 screen_coord_to_clip_coord(glm::vec2 pos)
{
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  glm::vec2 ss(window.Width() / 2, window.Height() / 2);
  pos /= ss;
  return pos;
}

template<typename T>
void main_loop(T func)
{
  const glm::vec2 invColorFilterSize = glm::vec2(1, 1) / glm::vec2(colorFilter.Texture()->Width(), colorFilter.Texture()->Height());

  GLFWEW::Window& window = GLFWEW::Window::Instance();
  for (;;) {
    Audio::Engine::Get().Update();
    window.Update();
    const GamePad gamepad = window.GetGamePad();
    if (window.ShouldClose() || (gamepad.buttonDown & GamePad::ESC)) {
      quit();
      exit(0);
      break;
    }

    if (func()) {
      break;
    }

    const float deltaTime = window.DeltaTime();
    rootNode.UpdateTransform();
    rootNode.UpdateRecursive(deltaTime);
    spriteRenderer.Update(rootNode);

    const glm::vec2 windowSize(window.Width(), window.Height());

    colorFilter.UpdateTransform();
    colorFilter.UpdateRecursive(deltaTime);
    colorFilter.Scale(windowSize * invColorFilterSize);
    colorFilterRenderer.Update(colorFilter);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    spriteRenderer.Draw(windowSize);
    fontRenderer.Draw();
    colorFilterRenderer.Draw(windowSize);

    window.SwapBuffers();
  }
}

void initialize()
{
  setlocale(LC_CTYPE, "JPN");
  Texture::Initialize();

  static const uint32_t planeTexData[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
  };

  spriteRenderer.Init(1024);
  spriteBuffer.resize(1024);
  for (auto&& e : spriteBuffer) {
    rootNode.AddChild(&e);
  }

  colorFilterRenderer.Init(1);
  colorFilter.Texture(Texture::Create(4, 4, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, planeTexData));
  colorFilter.Color(glm::vec4(0, 0, 0, 0));
  colorFilter.action.init(0, 0, linear, 0);

  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  fontRenderer.Init(1024, glm::vec2(window.Width(), window.Height()));
  fontRenderer.LoadFromFile("Res/Font/font.fnt");

  randomEngine.seed(std::random_device()());
}

void set_text(float x, float y, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  char tmp[1024];
  vsnprintf(tmp, sizeof(tmp), format, ap);
  va_end(ap);

  textList.push_back({ screen_coord_to_clip_coord(glm::vec2(x, y)), sjis_to_utf16(tmp) });
}

void reset_all_text()
{
  textList.clear();
}

void set_image(int no, float x, float y, const char* filename)
{
  if (no < 0 || no >= static_cast<int>(spriteBuffer.size())) {
    return;
  }
  std::string str;
  str.reserve(1024);
  str += "Res/";
  str += filename;
  if (TexturePtr tex = Texture::LoadFromFile(str.c_str())) {
    spriteBuffer[no].Texture(tex);
    spriteBuffer[no].Position(glm::vec3(x, y, 0));
    spriteBuffer[no].Scale(glm::vec2(1, 1));
    spriteBuffer[no].Rotation(0);
    spriteBuffer[no].Color(glm::vec4(1, 1, 1, 1));
    spriteBuffer[no].init_action();
  }
}

void move_image(int no, float x, float y, int easing, float seconds)
{
  auto& e = spriteBuffer[no];
  e.translate.init(e.Position(), glm::vec2(x, y), easing, seconds);
  e.Position(glm::vec3(e.translate.update(0), 0));
}

void scale_image(int no, float x, float y, int easing, float seconds)
{
  auto& e = spriteBuffer[no];
  e.scale.init(e.Scale(), glm::vec2(x, y), easing, seconds);
  e.Scale(e.scale.update(0));
}

void rotate_image(int no, float degree, int easing, float seconds)
{
  auto& e = spriteBuffer[no];
  e.rotate.init(e.Rotation(), degree * (glm::pi<float>() / 180.0f), easing, seconds);
  e.Rotation(e.rotate.update(0));
}

void color_blend_image(int no, float red, float green, float blue, float alpha, int mode, int easing, float seconds)
{
  auto& e = spriteBuffer[no];
  e.ColorMode(static_cast<BlendMode>(glm::clamp(mode, 0, 2)));
  e.color.init(e.Color(), glm::vec4(red, green, blue, alpha), easing, seconds);
  e.Color(e.color.update(0));
}

void reset_image(int no)
{
  spriteBuffer[no].Texture({});
}

void reset_all_image()
{
  for (auto& e : spriteBuffer) {
    e.Texture({});
  }
}

void fade_out(float red, float green, float blue, float seconds)
{
  colorFilter.Color(glm::vec4(red, green, blue, 0));
  colorFilter.action.init(0, 1, linear, seconds);
  wait(seconds);
}

void fade_in(float seconds)
{
  colorFilter.action.init(1, 0, linear, seconds);
  wait(seconds);
}

void wait(float seconds)
{
  fontRenderer.Color(glm::vec4(1));
  fontRenderer.MapBuffer();
  for (const auto& e : textList) {
    fontRenderer.AddString(e.pos, e.text.c_str());
  }
  fontRenderer.UnmapBuffer();

  main_loop([&seconds] {
    GLFWEW::Window& window = GLFWEW::Window::Instance();
    seconds -= window.DeltaTime();
    return seconds <= 0;
  });
}

int select(float x, float y, int count, const char* a, const char* b, ...)
{
  std::vector<std::wstring> selectionList;

  va_list ap;
  va_start(ap, b);
  selectionList.push_back(sjis_to_utf16(a));
  selectionList.push_back(sjis_to_utf16(b));
  for (int i = 2; i < count; ++i) {
    const char* p = va_arg(ap, const char*);
    selectionList.push_back(sjis_to_utf16(p));
  }
  va_end(ap);

  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(glm::vec2(x, y));
  int select = 0;
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  main_loop([&] {
    const GamePad gamepad = window.GetGamePad();
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (--select < 0) {
        select = static_cast<int>(selectionList.size() - 1);
      }
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (++select >= static_cast<int>(selectionList.size())) {
        select = 0;
      }
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      return true;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    glm::vec2 textPos(textPosOrigin);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < static_cast<int>(selectionList.size()); ++i) {
      fontRenderer.Color(i == select ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
      fontRenderer.AddString(textPos, selectionList[i].c_str());
      textPos.y -= nextLineOffset;
    }
    fontRenderer.UnmapBuffer();
    return false;
  });
  return select;
}

int random(int min, int max)
{
  return std::uniform_int_distribution<>(min, max)(randomEngine);
}

void quit()
{
  bgm.reset();
  hasQuitRequest = true;
  Texture::Finalize();
}

void play_sound(const char* filename)
{
  std::string str;
  str.reserve(1024);
  str += "Res/";
  str += filename;
  const std::wstring ws = sjis_to_utf16(str.c_str());
  Audio::SoundPtr p = Audio::Engine::Get().PrepareMFStream(ws.c_str());
  p->Play(Audio::Flag_None);
  p->SetVolume(seVolume);
}

void play_bgm(const char* filename)
{
  if (bgmFilename != filename || !bgm || !(bgm->GetState() & Audio::State_Playing)) {
    if (bgm) {
      bgm->Stop();
    }
    bgmFilename = filename;
    std::string str;
    str.reserve(1024);
    str += "Res/";
    str += filename;
    const std::wstring ws = sjis_to_utf16(str.c_str());
    bgm = Audio::Engine::Get().PrepareMFStream(ws.c_str());
    bgm->Play(Audio::Flag_Loop);
    bgm->SetVolume(bgmVolume);
  }
}

void stop_bgm()
{
  bgmFilename.clear();
  if (bgm) {
    bgm->Stop();
    bgm.reset();
  }
}

void set_sound_volume(float volume)
{
  seVolume = volume;
}

void set_bgm_volume(float volume)
{
  bgmVolume = volume;
  if (bgm) {
    bgm->SetVolume(volume);
  }
}
