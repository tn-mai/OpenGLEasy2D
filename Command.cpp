/**
* @file Command.cpp
*/
#include "Command.h"
#include "GLFWEW.h"
#include "Sprite.h"
#include "Font.h"
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <iostream>
#include <stdarg.h>

bool hasQuitRequest = false;

namespace /* unnamed */ {

Sprite rootNode;
std::vector<Sprite> spriteBuffer;
SpriteRenderer spriteRenderer;

glm::vec2 textOrigin;
int maxTextLines;

struct text_info
{
  glm::vec2 pos;
  std::wstring text;
};
std::vector<text_info> textList;
Font::Renderer fontRenderer;

std::mt19937 randomEngine;

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

void initialize()
{
  setlocale(LC_CTYPE, "JPN");
  Texture::Initialize();
  spriteRenderer.Init(1024);
  spriteBuffer.resize(1024);
  for (auto&& e : spriteBuffer) {
    rootNode.AddChild(&e);
  }
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  fontRenderer.Init(1024, glm::vec2(window.Width(), window.Height()));
  fontRenderer.LoadFromFile("Res/Font/font.fnt");

  randomEngine.seed(std::random_device()());
}

void set_text(int x, int y, const char* format, ...)
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
  }
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

void wait(float seconds)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  for (;;) {
    window.Update();
    seconds -= window.DeltaTime();
    if (seconds <= 0) {
      return;
    }
    if (window.ShouldClose() || (window.GetGamePad().buttonDown & GamePad::ESC)) {
      quit();
      exit(0);
      return;
    }

    rootNode.UpdateTransform();
    rootNode.UpdateRecursive(window.DeltaTime());
    spriteRenderer.Update(rootNode);

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }
    fontRenderer.UnmapBuffer();

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    spriteRenderer.Draw(glm::vec2(window.Width(), window.Height()));
    fontRenderer.Draw();

    window.SwapBuffers();
  }
}

int select(int x, int y, int count, const char* a, const char* b, ...)
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
  for (;;) {
    window.Update();
    const GamePad gamepad = window.GetGamePad();
    if (window.ShouldClose() || (gamepad.buttonDown & GamePad::ESC)) {
      quit();
      exit(0);
      return 0;
    }

    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (--select < 0) {
        select = static_cast<int>(selectionList.size() - 1);
      }
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (++select >= static_cast<int>(selectionList.size())) {
        select = 0;
      }
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      break;
    }

    rootNode.UpdateTransform();
    rootNode.UpdateRecursive(window.DeltaTime());
    spriteRenderer.Update(rootNode);

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    glm::vec2 textPos(textPosOrigin);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < static_cast<int>(selectionList.size()); ++i) {
      fontRenderer.Color(i == select ? glm::vec4(1,1,1,1) : glm::vec4(0.5f, 0.5f, 0.5f,1));
      fontRenderer.AddString(textPos, selectionList[i].c_str());
      textPos.y -= nextLineOffset;
    }
    fontRenderer.UnmapBuffer();

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    spriteRenderer.Draw(glm::vec2(window.Width(), window.Height()));
    fontRenderer.Draw();

    window.SwapBuffers();
  }
  return select;
}

int random(int min, int max)
{
  return std::uniform_int_distribution<>(min, max)(randomEngine);
}

void quit()
{
  hasQuitRequest = true;
  Texture::Finalize();
}