/**
* @file Application.cpp
*/
#include "Command.h"

// 一度タイトル画面を表示するとfalseになる.
bool title_flag = true;

// 前回「あいこ」のときtrue、そうでなければfalseが格納される.
// trueは「真」、falseは「偽」という意味.
// ひとつのイコール記号は代入を意味する.
bool aiko_flag = false;

// プレイヤーの現在位置.
int player_x;
int player_y;

// プレイヤーの向き.
const int dir_up = 0;
const int dir_right = 1;
const int dir_down = 2;
const int dir_left = 3;
int player_direction;

// プレイヤーの最大ヒットポイント.
const int player_hp_max = 10;

// プレイヤーのヒットポイント.
int player_hp;

// ダンジョンの大きさ
const int dungeon_width = 8;
const int dungeon_height = 8;

/**
* ダンジョンマップ.
*
* 0: 通路
* 1: 壁
*/
const char dungeon_map[dungeon_width][dungeon_height] = {
{ 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 0, 1, 0, 1, 0, 0, 1 },
{ 1, 0, 1, 2, 1, 1, 0, 1 },
{ 1, 0, 0, 0, 1, 0, 0, 1 },
{ 1, 0,-1, 0, 0, 0, 1, 1 },
{ 1, 1, 1, 0, 1, 0, 0, 1 },
{ 1, 0, 0, 0, 0, 0, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1 },
};

// スタートの位置.
const int start_x = 1;
const int start_y = 1;

// ゴールの位置.
const int goal_x = 3;
const int goal_y = 1;

// 鍵の位置.
const int key_x = 5;
const int key_y = 1;

// 鍵を持っているか.
bool has_key = false;

// 戦闘中のときはtrue. そうでなければfalse.
bool battle_flag;

// 敵との遭遇確率.
int encount_percent = 0;

/**
* アプリケーションの本体.
*/
void application()
{
  // タイトル画面を表示
  if (title_flag) {
    /*
    * タイトル画面.
    */
    reset_all_image();
    reset_all_text();
    set_image(0, 0, 0, "janken_background.png"); // 背景を表示.
    set_image(1, 0, 500, "janken_title.png");
    move_image(1, 0, 50, 5, 1.5);
    scale_image(1, 0.5, 2, 0, 0);
    scale_image(1, 1, 1, 5, 1.5);
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 0.5);
    wait(2);
    const int yes_or_no = select(-80, -80, 2, "はじめる", "やめる");
    play_sound("switch1.mp3");
    // "やめる"が選ばれたらアプリケーションを終了する.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // アプリケーションを終了させる.
    } else {
      title_flag = false;
      fade_out(0, 0, 0, 1);
      reset_all_text();
      reset_all_image();
      play_bgm("Prelude and Action.mp3");
      fade_in(1);

      // ゲームの初期設定を行う.
      player_x = start_x;
      player_y = start_y;
      player_direction = dir_down;
      player_hp = player_hp_max;
      aiko_flag = false;
      battle_flag = false;
    }
  } else if (!battle_flag) {
    /*
    * ダンジョン探索.
    */
    reset_all_image();
    reset_all_text();

    /*
    + 方向に応じて壁の有無を調べる.
    * 調査範囲は前方4マス、左右2マス.
    */
    const int vicinity_width = 5;
    const int vicinity_height = 5;
    int vicinity_map[vicinity_height][vicinity_width];
    {
      const int check_start_offset[4][2] = { {-2, -4},{ 4, -2 },{ 2, 4 },{ -4, 2 } };
      const int check_advance_x[4][2] = { { 1, 0 },{ 0, 1 },{ -1, 0 },{ 0, -1 } };
      const int check_advance_y[4][2] = { { 0, 1 },{ -1, 0 },{ 0, -1 },{ 1, 0 } };
      int x_base = player_x + check_start_offset[player_direction][0];
      int y_base = player_y + check_start_offset[player_direction][1];
      for (int i = 0; i < 5; ++i) {
        int x = x_base;
        int y = y_base;
        for (int j = 0; j < 5; ++j) {
          if (x < 0 || x >= dungeon_width || y < 0 || y >= dungeon_height) {
            vicinity_map[i][j] = 1;
          } else {
            vicinity_map[i][j] = dungeon_map[y][x];
          }
          x += check_advance_x[player_direction][0];
          y += check_advance_x[player_direction][1];
        }
        x_base += check_advance_y[player_direction][0];
        y_base += check_advance_y[player_direction][1];
      }
    }

    /*
    * -1(泉) : 床と天井と泉を描く
    * 0(通路): 床と天井を描く
    * 1(壁): 壁を描く
    * 2(扉): 壁と扉を描く
    *
    * 管理番号は左奥から割り振る.
    */

    // 壁のある部分に画像を配置.
    const float base_scales[vicinity_height] = { 0.0625, 0.125, 0.25, 0.5, 1 };
    const float ceil_shears[vicinity_width] = { -4, -2, 0, 2, 4 };
    const float floor_shears[vicinity_width] = { 4, 2, 0, -2, -4 };
    const float wall_x_scales[vicinity_width] = { 3, 1, 0, -1, -3 };
    const float ceil_offsets[vicinity_width][2] = { { -1536, 384 },{ -768, 384 },{ 0, 384 },{ 768, 384 },{ 1536, 384 } };
    const float floor_offsets[vicinity_width][2] = { { -1536, -384 },{ -768, -384 },{ 0, -384 },{ 768, -384 },{ 1536, -384 } };
    const float floor_object_offsets[vicinity_width][2] = { { -1536, 0 },{ -768, 0 },{ 0, 0 },{ 768, 0 },{ 1536, 0 } };
    const float wall_front_offsets[vicinity_width][2] = { { -1024, 0 },{ -512, 0 },{ 0, 0 },{ 512, 0 },{ 1024, 0 } };
    const float wall_side_offsets[vicinity_width][2] = { { -1152, 0 },{ -384, 0 },{ 0, 0 },{ 384, 0 },{ 1152, 0 } };
    const int x_order[vicinity_width] = {0, 1, 4, 3, 2};
    const float luminances[vicinity_height][vicinity_width] = {
      { 0.05f, 0.10f, 0.13f, 0.10f, 0.05f },
      { 0.19f, 0.31f, 0.35f, 0.31f, 0.19f },
      { 0.39f, 0.52f, 0.57f, 0.52f, 0.39f },
      { 0.52f, 0.70f, 0.80f, 0.70f, 0.52f },
      { 0.57f, 0.80f, 1.00f, 0.80f, 0.57f },
    };
    int image_no = 1;
    set_image(0, 0, 0, "dungeon_background.png");
    for (int i = 0; i < vicinity_height; ++i) {
      const float base_scale = base_scales[i];
      for (int x = 0; x < vicinity_width; ++x) {
        const int j = x_order[x];
        const int wall_type = vicinity_map[i][j];
        const float lum = luminances[i][j];
        set_image(image_no, 0, 0, "dungeon_ceil.png");
        move_image(image_no, ceil_offsets[j][0] * base_scale, ceil_offsets[j][1] * base_scale, 0, 0);
        scale_image(image_no, base_scale, base_scale, 0, 0);
        shear_image(image_no, ceil_shears[j], 0, 0);
        color_blend_image(image_no, lum, lum, lum, 1, 0, 0, 0);
        ++image_no;
        set_image(image_no, 0, 0, "dungeon_floor.png");
        move_image(image_no, floor_offsets[j][0] * base_scale, floor_offsets[j][1] * base_scale, 0, 0);
        scale_image(image_no, base_scale, base_scale, 0, 0);
        shear_image(image_no, floor_shears[j], 0, 0);
        color_blend_image(image_no, lum, lum, lum, 1, 0, 0, 0);
        ++image_no;
        if (wall_type == -1) {
          set_image(image_no, 0, 0, "dungeon_spring.png");
          move_image(image_no, floor_object_offsets[j][0] * base_scale, floor_object_offsets[j][1] * base_scale, 0, 0);
          scale_image(image_no, base_scale, base_scale, 0, 0);
          color_blend_image(image_no, lum, lum, lum, 1, 0, 0, 0);
          ++image_no;
        } else if (wall_type > 0) {
          if (i < vicinity_height - 1) {
            const float front_lum = luminances[i + 1][j];
            const float front_scale = base_scale * 2;
            set_image(image_no, 0, 0, "dungeon_wall_front.png");
            move_image(image_no, wall_front_offsets[j][0] * front_scale, wall_front_offsets[j][1] * front_scale, 0, 0);
            scale_image(image_no, front_scale, front_scale, 0, 0);
            color_blend_image(image_no, front_lum, front_lum, front_lum, 1, 0, 0, 0);
            ++image_no;
            if (wall_type == 2) {
              set_image(image_no, 0, 0, "dungeon_door_front.png");
              move_image(image_no, wall_front_offsets[j][0] * front_scale, wall_front_offsets[j][1] * front_scale, 0, 0);
              scale_image(image_no, front_scale, front_scale, 0, 0);
              color_blend_image(image_no, front_lum, front_lum, front_lum, 1, 0, 0, 0);
              ++image_no;
            }
          }
          if (j != 2) {
            const float x_scale = wall_x_scales[j] * base_scale;
            set_image(image_no, 0, 0, "dungeon_wall_side.png");
            move_image(image_no, wall_side_offsets[j][0] * base_scale, wall_side_offsets[j][1] * base_scale, 0, 0);
            scale_image(image_no, x_scale, base_scale, 0, 0);
            color_blend_image(image_no, lum, lum, lum, 1, 0, 0, 0);
            ++image_no;
            if (wall_type == 2) {
              set_image(image_no, 0, 0, "dungeon_door_side.png");
              move_image(image_no, wall_side_offsets[j][0] * base_scale, wall_side_offsets[j][1] * base_scale, 0, 0);
              scale_image(image_no, x_scale, base_scale, 0, 0);
              color_blend_image(image_no, lum, lum, lum, 1, 0, 0, 0);
              ++image_no;
            }
          }
        }
      }
    }

    // 現在位置と向きを表示.
    const char* const direction_text[] = { "北", "東", "南", "西" };
    set_text(-360, 260, "(%d, %d) %s", player_x, player_y, direction_text[player_direction]);

    // 確率で敵と遭遇.
    if (random(0, 99) < encount_percent) {
      set_text(-360, 0, "怪物に見つかった！");
      wait(2);
      battle_flag = true;
    } else if (player_x == goal_x && player_y == goal_y) {
      // ゴールに到達したのでメッセージを表示してタイトルに戻る.
      set_text(-360, 0, "出口だ！");
      set_text(-360, -40, "おめでとう！");
      set_text(-360, -80, "あなたはダンジョンからの脱出に成功した！");
      set_text(-360, -120, "ＧＡＭＥ　ＣＬＥＡＲ");
      wait(1);
      set_text(-360, -160, "(何かキーを押すとタイトルに戻ります)");
      title_flag = true;
      wait_any_key();
      stop_bgm();
      wait(1);
    } else {
      // ゴールではないので行動を選択する.
      if (!has_key && player_x == key_x && player_y == key_y) {
        set_text(-360, 0, "鍵が落ちている.");
        wait(1);
        set_text(-360, -40, "鍵を拾いました.");
        set_text(-360, -80, "(何かキーを押してください)");
        has_key = true;
        wait_any_key();
        reset_text_area(-400, -300, 800, 301);
      }
      set_text(-360, 0, "どうしますか？");
      const int player_action = select(-360, -40, 4, "前進", "右を向く", "左を向く", "後ろを向く");
      if (player_action == 0) {
        const int front_position[2] = { 2, 3 };
        const int wall_type = vicinity_map[front_position[1]][front_position[0]];
        bool can_go_ahead = false;
        if (wall_type <= 0) {
          can_go_ahead = true;
        } else if (wall_type == 1) {
          // 前方に壁がある場合は進めない.
          reset_text_area(-400, -200, 800, 301);
          set_text(-360, 0, "壁があって進めない");
          wait(1.5);
          can_go_ahead = false;
        } else if (wall_type == 2) {
          if (!has_key) {
            reset_text_area(-400, -300, 800, 301);
            set_text(-360, 0, "鍵がかかっている.");
            wait(1.5);
            can_go_ahead = false;
          } else {
            reset_text_area(-400, -200, 800, 301);
            set_text(-360, 0, "鍵を開けた.");
            wait(1.5);
            can_go_ahead = true;
          }
        }
        if (can_go_ahead) {
          // 壁がなければ、プレイヤーの前方へ移動する.
          const int move[4][2] = {{ 0, -1 },{ 1, 0 },{ 0, 1 },{ -1, 0 }};
          player_x += move[player_direction][0];
          player_y += move[player_direction][1];
        }
      } else {
        // プレイヤーの向きを変える.
        const int rotation_count[] = { 0, 1, 3, 2 };
        player_direction += rotation_count[player_action];
        if (player_direction >= 4) {
          player_direction -= 4;
        }
      }
    }
  } else {
    /*
    * じゃんけんバトル.
    */
    reset_all_text();
    set_text(-360, 260, "あなた(HP %d/%d)", player_hp, player_hp_max);
    set_image(100, 0, -100, "goblin.png");
    scale_image(100, 0.5, 0.5, 0, 0);

    // あいこフラグがtrueなら"あいこで"画像を表示. falseなら"じゃんけん"画像を表示.
    const char* aiko_images[] = { "janken.png", "janken_aikode.png" };
    set_image(101, 0, 400, aiko_images[aiko_flag]);
    move_image(101, 0, 100, 4, 0.75);
    wait(0.75);

    set_text(-360, 0, "手を選んでください");

    // プレイヤーの手を選んでもらう.
    // 選ばれたのがグーなら0が、チョキなら1が、パーなら2がplayer_handに格納される.
    const int player_hand = select(-360, -40, 3, "グー", "チョキ", "パー");

    // CPUの手を選ぶ.
    // 選んだのがグーなら0が、チョキなら1が、パーなら2がcpu_handに格納される.
    const int cpu_hand = random(0, 2); // 0～2のいずれかの整数が無作為に選ばれる.

    reset_image(101); // "じゃんけん"(または"あいこで")画像を消す.

    const char* janken_images[] = { "janken_gu.png", "janken_choki.png", "janken_pa.png" };
    // 左側にプレイヤーの手を表示
    set_image(102, -200, 100, janken_images[player_hand]);
    scale_image(102, 0, 0, 0, 0);
    scale_image(102, 1, 1, 4, 0.25f);

    // 右側にCPUの手を表示
    set_image(103, 200, 100, janken_images[cpu_hand]);
    scale_image(103, 0, 0, 0, 0);
    scale_image(103, 1, 1, 4, 0.25f);

    play_sound("kotsudumi1.mp3");
    wait(2); // 2秒間待つ

    // 数値の比較は二重のイコール記号「==」で行う.
    // 「&&」は「且つ」、「||」は「又は」という意味を持つ記号.
    // 式の優先順位を調整するには算数と同様にカッコ「()」を使う.
    // グーは0、チョキは1、パーは2なので、例えばプレイヤーの手が0(グー)でCPUの手が1(チョキ)なら、プレイヤーの勝ちとなる.
    // 勝ったり負けたりした場合は「あいこ」じゃないのでaiko_flagを「偽」にしておく.
    // 勝ってもいないし負けてもいない場合は「あいこ」なのでaiko_flagを「真」にしておく.
    bool win_flag = false;
    if ((player_hand == 0 && cpu_hand == 1) || (player_hand == 1 && cpu_hand == 2) || (player_hand == 2 && cpu_hand == 0)) {
      play_sound("correct4.mp3");
      set_image(104, 0, -150, "janken_kachi.png");
      win_flag = true;
      aiko_flag = false;
    } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
      play_sound("incorrect1.mp3");
      set_image(104, 0, -150, "janken_make.png");
      player_hp -= 1;
      aiko_flag = false;
    } else {
      play_sound("stupid2.mp3");
      aiko_flag = true;
    }

    reset_all_text(); // いったんすべての文字を消す.

    // ヒットポイントが変化した可能性があるので表示しなおす.
    set_text(-360, 260, "あなた(HP %d/%d)", player_hp, player_hp_max);

    if (win_flag) {
      battle_flag = false;
      set_text(-360, 0, "怪物に勝った！");
      wait(2);
    } else if (player_hp <= 0) {
      battle_flag = false;
      set_text(-360, 0, "あなたは怪物にやられてしまった…");
      set_text(-360, -40, "ＧＡＭＥ　ＯＶＥＲ");
      wait(1);
      set_text(-360, -80, "(何かキーを押すとタイトルに戻ります)");
      wait_any_key();
      wait(1);
      title_flag = true;
    } else {
      wait(1);
    }
    for (int i = 100; i < 200; ++i) {
      reset_image(i);
    }
  }
}
