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
#include <algorithm>

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
* utf16文字列からsjis文字列を得る.
*/
std::string utf16_to_sjis(const std::wstring& p)
{
  std::string tmp;
  tmp.resize(p.size() * 2);
  wcstombs(&tmp[0], p.c_str(), tmp.size());
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

    colorFilter.Scale(windowSize * invColorFilterSize);
    colorFilter.UpdateTransform();
    colorFilter.UpdateRecursive(deltaTime);
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

void reset_text_area(float x, float y, float width, float height)
{
  const glm::vec2 min = screen_coord_to_clip_coord(glm::vec2(x, y));
  const glm::vec2 max = screen_coord_to_clip_coord(glm::vec2(x + width, y + height));
  const auto itr = std::remove_if(textList.begin(), textList.end(), [min, max](const text_info& e) {
    return (e.pos.x >= min.x) && (e.pos.x < max.x) && (e.pos.y >= min.y) && (e.pos.y < max.y);
  });
  textList.erase(itr, textList.end());
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
    spriteBuffer[no].ColorMode(BlendMode_Multiply);

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

void wait_any_key()
{
  fontRenderer.Color(glm::vec4(1));
  fontRenderer.MapBuffer();
  for (const auto& e : textList) {
    fontRenderer.AddString(e.pos, e.text.c_str());
  }
  fontRenderer.UnmapBuffer();

  main_loop([] {
    GLFWEW::Window& window = GLFWEW::Window::Instance();
    const GamePad gamepad = window.GetGamePad();
    return gamepad.buttons || (window.NumOfKeyPressed() > 0);
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
  float timer = 0;
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  main_loop([&] {
    const GamePad gamepad = window.GetGamePad();
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (--select < 0) {
        select = static_cast<int>(selectionList.size() - 1);
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (++select >= static_cast<int>(selectionList.size())) {
        select = 0;
      }
      timer = 0;
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      return true;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    glm::vec2 textPos(textPosOrigin);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < static_cast<int>(selectionList.size()); ++i) {
      fontRenderer.Color(i == select && timer < 0.5f ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
      fontRenderer.AddString(textPos, selectionList[i].c_str());
      textPos.y -= nextLineOffset;
    }
    fontRenderer.UnmapBuffer();
    return false;
  });
  return select;
}

int select_number(float x, float y, int min, int max)
{
  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(glm::vec2(x, y));
  int select = 0;
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  float timer = 0;
  main_loop([&] {
    const GamePad gamepad = window.GetGamePad();
    const int unit = (gamepad.buttons & (GamePad::L | GamePad::R)) ? 10 : 1;
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      select -= unit;
      if (select < min) {
        select = min;
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      select += unit;
      if (select > max) {
        select = max;
      }
      timer = 0;
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      return true;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    std::wstring str;
    int i = select;
    for (int j = max; j > 0; j /= 10) {
      str.push_back(L'0' + i % 10);
      i /= 10;
    }
    std::reverse(str.begin(), str.end());

    fontRenderer.Color(timer < 0.5f ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
    fontRenderer.AddString(textPosOrigin, str.c_str());
    fontRenderer.UnmapBuffer();
    return false;
  });
  return select;
}

void select_string(float x, float y, int max, char* buffer)
{
  enum {
    kana,
    dakuten,
    han_dakuten,
    minus,
    space,
    kana_switch,
    submit,
  };
  struct char_data {
    wchar_t str[4];
    wchar_t daku, handaku;
    int8_t x, y;
    int8_t type;
  };
  static const char_data charTable[] = {
  { L"あ", 0, 0, 0, 0, kana },    { L"い", 0, 0, 1, 0, kana },    { L"う", 0, 0, 2, 0, kana },    { L"え", 0, 0, 3, 0, kana },    { L"お", 0, 0, 4, 0, kana },    { L"は", L'ば', L'ぱ', 6, 0, kana },{ L"ひ", L'び', L'ぴ', 7, 0, kana },{ L"ふ", L'ぶ', L'ぷ', 8, 0, kana },{ L"へ", L'べ', L'ぺ', 9, 0, kana },{ L"ほ", L'ぼ', L'ぽ', 10, 0, kana },{ L"゛", 0, 0, 12, 0, dakuten },{ L"゜", 0, 0, 13, 0, han_dakuten },
  { L"か", L'が', 0, 0, 1, kana },{ L"き", L'ぎ', 0, 1, 1, kana },{ L"く", L'ぐ', 0, 2, 1, kana },{ L"け", L'げ', 0, 3, 1, kana },{ L"こ", L'ご', 0, 4, 1, kana },{ L"ま", 0, 0, 6, 1, kana },        { L"み", 0, 0, 7, 1, kana },        { L"む", 0, 0, 8, 1, kana },        { L"め", 0, 0, 9, 1, kana },        { L"も", 0, 0, 10, 1, kana },        { L"－", 0, 0, 12, 1, minus },
  { L"さ", L'ざ', 0, 0, 2, kana },{ L"し", L'じ', 0, 1, 2, kana },{ L"す", L'ず', 0, 2, 2, kana },{ L"せ", L'ぜ', 0, 3, 2, kana },{ L"そ", L'ぞ', 0, 4, 2, kana },{ L"ら", 0, 0, 6, 2, kana },        { L"り", 0, 0, 7, 2, kana },        { L"る", 0, 0, 8, 2, kana },        { L"れ", 0, 0, 9, 2, kana },        { L"ろ", 0, 0, 10, 2, kana },        { L"空白", 0, 0, 12, 2, space },
  { L"た", L'だ', 0, 0, 3, kana },{ L"ち", L'ぢ', 0, 1, 3, kana },{ L"つ", L'づ', 0, 2, 3, kana },{ L"て", L'で', 0, 3, 3, kana },{ L"と", L'ど', 0, 4, 3, kana },{ L"や", 0, 0, 6, 3, kana },        { L"ゆ", 0, 0, 7, 3, kana },        { L"よ", 0, 0, 8, 3, kana },        { L"わ", 0, 0, 9, 3, kana },        { L"ん", 0, 0, 10, 3, kana },        { L"カナ", 0, 0, 12, 3, kana_switch },
  { L"な", L'な', 0, 0, 4, kana },{ L"に", 0, 0, 1, 4, kana},     { L"ぬ", 0, 0, 2, 4, kana},     { L"ね", 0, 0, 3, 4, kana},     { L"の", 0, 0, 4, 4, kana},     { L"ゃ", 0, 0, 6, 4, kana },        { L"ゅ", 0, 0, 7, 4, kana },        { L"ょ", 0, 0, 8, 4, kana },        { L"っ", 0, 0, 9, 4, kana },        { L"を", 0, 0, 10, 4, kana },        { L"決定", 0, 0, 12, 4, submit},
  };
  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(glm::vec2(x, y));
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  int select = 0;
  float timer = 0;
  bool isKatakana = false;
  std::wstring tmpBuffer;
  std::vector<char_data> tmpDataBuffer;
  tmpBuffer.reserve(max);
  tmpDataBuffer.reserve(max);
  main_loop([&] {
    static const int kana_offset = L'ア' - L'あ';
    const GamePad gamepad = window.GetGamePad();
    if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      const int type = charTable[select].type;
      if (type == submit) {
        if (!tmpBuffer.empty()) {
          const std::string tmp = utf16_to_sjis(tmpBuffer);
          std::copy_n(tmp.c_str(), tmp.size() + 1, buffer);
          return true;
        }
      } else if (type == kana_switch) {
        isKatakana = !isKatakana;
      } else if (static_cast<int>(tmpBuffer.size()) < max) {
        if (type == space) {
          tmpBuffer.push_back(L'　');
          tmpDataBuffer.push_back(charTable[select]);
        } else if (type == minus) {
          tmpBuffer.push_back(L'－');
          tmpDataBuffer.push_back(charTable[select]);
        } else if (type == dakuten) {
          const char_data& data = tmpDataBuffer.back();
          if (data.daku && !tmpBuffer.empty()) {
            wchar_t c = tmpBuffer.back();
            bool isKana = false;
            if (c == data.str[0] + kana_offset) {
              c -= kana_offset;
              isKana = true;
            }
            if (c == data.daku) {
              c = data.str[0];
            } else {
              c = data.daku;
            }
            if (isKana) {
              c += kana_offset;
            }
            tmpBuffer.back() = c;
          }
        } else if (type == han_dakuten) {
          const char_data& data = tmpDataBuffer.back();
          if (data.handaku && !tmpBuffer.empty()) {
            wchar_t c = tmpBuffer.back();
            bool isKana = false;
            if (c == data.str[0] + kana_offset) {
              c -= kana_offset;
              isKana = true;
            }
            if (c == data.handaku) {
              c = data.str[0];
            } else {
              c = data.handaku;
            }
            if (isKana) {
              c += L'ア' - L'あ';
            }
            tmpBuffer.back() = c;
          }
        } else {
          wchar_t c = charTable[select].str[0];
          if (isKatakana) {
            c += L'ア' - L'あ';
          }
          tmpBuffer.push_back(c);
          tmpDataBuffer.push_back(charTable[select]);
        }
      }
    } else if (gamepad.buttonDown & GamePad::B) {
      tmpBuffer.pop_back();
      tmpDataBuffer.pop_back();
    }
    if (gamepad.buttonDown & GamePad::DPAD_LEFT) {
      if (select < 12) {
        if (select == 0) {
          select = 11;
        } else {
          --select;
        }
      } else {
        if ((select - 12) % 11 == 0) {
          select += 10;
        } else {
          --select;
        }
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_RIGHT) {
      if (select < 12) {
        if (select == 11) {
          select = 0;
        } else {
          ++select;
        }
      } else {
        if ((select - 12) % 11 == 10) {
          select -= 10;
        } else {
          ++select;
        }
      }
      timer = 0;
    }
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (select < 12) {
        if (select == 11) {
          select = 12 + 11 * 4 - 1;
        } else {
          select += 12 + 11 * 3;
        }
      } else if (select < 12 + 11) {
        select -= 12;
      } else {
        select -= 11;
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (select >= 12 + 11 * 3) {
        select -= 12 + 11 * 3;
      } else if (select == 11) {
        select += 11;
      } else if (select < 12) {
        select += 12;
      } else {
        select += 11;
      }
      timer = 0;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    std::wstring dispBuffer(tmpBuffer);
    dispBuffer.insert(dispBuffer.end(), max - tmpBuffer.size(), L'＊');
    fontRenderer.AddString(textPosOrigin, dispBuffer.c_str());
  
    const float nextCharOffset = 32.0f / static_cast<float>(window.Width() / 2);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < sizeof(charTable) / sizeof(charTable[0]); ++i) {
      glm::vec4 color(0.5f, 0.5f, 0.5f, 1);
      if (select == i && timer < 0.5f) {
        color = {1, 1, 1, 1};
      }
      fontRenderer.Color(color);
      const char_data& e = charTable[i];
      glm::vec2 textPos(textPosOrigin);
      textPos.x += e.x * nextCharOffset;
      textPos.y -= (e.y + 1) * nextLineOffset;
      if (isKatakana &&  e.str[1] == L'\0' && e.str[0] != L'゛' && e.str[0] != L'゜' && e.str[0] != L'－') {
        wchar_t str[2];
        str[0] = e.str[0] + kana_offset;
        str[1] = L'\0';
        fontRenderer.AddString(textPos, str);
      } else {
        fontRenderer.AddString(textPos, e.str);
      }
    }
    fontRenderer.UnmapBuffer();
    return false;
  });
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
