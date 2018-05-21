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

// 敵との遭遇確率.
const int encount_percent = 20;

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
{ 1, 0, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 1, 1, 1, 0, 1, 1 },
{ 1, 0, 0, 0, 1, 0, 0, 1 },
{ 1, 0, 1, 0, 1, 1, 0, 1 },
{ 1, 1, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 1, 0, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1 },
};

// スタートの位置.
const int start_x = 1;
const int start_y = 1;

// ゴールの位置.
const int goal_x = 3;
const int goal_y = 1;

// 戦闘中のときはtrue. そうでなければfalse.
bool battle_flag;

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
      play_bgm("073.mp3");
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

    // 方向に応じて壁の有無を調べる.
    int wall_left[2] = {};
    int wall_right[2] = {};
    int wall_front[1] = {};
    const int wall_check_list[][5][2] = {
    { {  0, -1 },{ -1, -1 },{  0,  1 },{ -1,  1 },{ -1,  0 } },
    { { -1,  0 },{ -1,  1 },{  1,  0 },{  1,  1 },{  0,  1 } },
    { {  0,  1 },{  1,  1 },{  0, -1 },{  1, -1 },{  1,  0 } },
    { {  1,  0 },{  1, -1 },{ -1,  0 },{ -1, -1 },{  0, -1 } },
    };
    wall_left[0]  = dungeon_map[player_y + wall_check_list[player_direction][0][0]][player_x + wall_check_list[player_direction][0][1]];
    wall_left[1]  = dungeon_map[player_y + wall_check_list[player_direction][1][0]][player_x + wall_check_list[player_direction][1][1]];
    wall_right[0] = dungeon_map[player_y + wall_check_list[player_direction][2][0]][player_x + wall_check_list[player_direction][2][1]];
    wall_right[1] = dungeon_map[player_y + wall_check_list[player_direction][3][0]][player_x + wall_check_list[player_direction][3][1]];
    wall_front[0] = dungeon_map[player_y + wall_check_list[player_direction][4][0]][player_x + wall_check_list[player_direction][4][1]];

    // 壁のある部分に画像を配置.
    set_image(0, 0, 0, "dungeon_background.png");
    if (wall_left[1]) {
      set_image(1, 0, 0, "dungeon_left_wall_1.png");
    }
    if (wall_left[0]) {
      set_image(2, 0, 0, "dungeon_left_wall_0.png");
    }
    if (wall_right[1]) {
      set_image(3, 0, 0, "dungeon_right_wall_1.png");
    }
    if (wall_right[0]) {
      set_image(4, 0, 0, "dungeon_right_wall_0.png");
    }
    if (wall_front[0]) {
      set_image(5, 0, 0, "dungeon_front_wall_0.png");
    }

    // 現在位置と向きを表示.
    const char* const direction_text[] = { "北", "東", "南", "西" };
    set_text(-360, 260, "(%d, %d) %s", player_x, player_y, direction_text[player_direction]);

    // 確率で敵と遭遇.
    if (random(0, 99) < encount_percent) {
      set_text(-360, 0, "怪物に見つかった！");
      wait(2);
      battle_flag = true;
    } else {
      if (player_x == goal_x && player_y == goal_y) {
        // ゴールに到達したのでメッセージを表示してタイトルに戻る.
        set_text(-360, 0, "出口だ！");
        set_text(-360, -40, "おめでとう！");
        set_text(-360, -80, "あなたはダンジョンからの脱出に成功した！");
        set_text(-360, -120, "ＧＡＭＥ　ＣＬＥＡＲ");
        wait(1);
        set_text(-360, -160, "(何かキーを押すとタイトルに戻ります)");
        title_flag = true;
        wait_any_key();
        wait(1);
      } else {
        // ゴールではないので行動を選択する.
        set_text(-360, 0, "どうしますか？");
        const int player_action = select(-360, -40, 4, "前進", "右を向く", "左を向く", "後ろを向く");
        if (player_action == 0) {
          if (wall_front[0]) {
            // 前方に壁がある場合は進めない.
            reset_text_area(-400, -200, 800, 301);
            set_text(-360, 0, "壁があって進めない");
            wait(1.5);
          } else {
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
    }
  } else {
    /*
    * じゃんけんバトル.
    */
    reset_all_text();
    set_text(-360, 260, "あなた(HP %d/%d)", player_hp, player_hp_max);
    set_image(10, 0, -100, "goblin.png");
    scale_image(10, 0.5, 0.5, 0, 0);

    // あいこフラグがtrueなら"あいこで"画像を表示. falseなら"じゃんけん"画像を表示.
    const char* aiko_images[] = { "janken.png", "janken_aikode.png" };
    set_image(11, 0, 400, aiko_images[aiko_flag]);
    move_image(11, 0, 100, 4, 0.75);
    wait(0.75);

    set_text(-360, 0, "手を選んでください");

    // プレイヤーの手を選んでもらう.
    // 選ばれたのがグーなら0が、チョキなら1が、パーなら2がplayer_handに格納される.
    const int player_hand = select(-360, -40, 3, "グー", "チョキ", "パー");

    // CPUの手を選ぶ.
    // 選んだのがグーなら0が、チョキなら1が、パーなら2がcpu_handに格納される.
    const int cpu_hand = random(0, 2); // 0～2のいずれかの整数が無作為に選ばれる.

    reset_image(11); // "じゃんけん"(または"あいこで")画像を消す.

    const char* janken_images[] = { "janken_gu.png", "janken_choki.png", "janken_pa.png" };
    // 左側にプレイヤーの手を表示
    set_image(12, -200, 100, janken_images[player_hand]);
    scale_image(12, 0, 0, 0, 0);
    scale_image(12, 1, 1, 4, 0.25f);

    // 右側にCPUの手を表示
    set_image(13, 200, 100, janken_images[cpu_hand]);
    scale_image(13, 0, 0, 0, 0);
    scale_image(13, 1, 1, 4, 0.25f);

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
      set_image(14, 0, -150, "janken_kachi.png");
      win_flag = true;
      aiko_flag = false;
    } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
      play_sound("incorrect1.mp3");
      set_image(14, 0, -150, "janken_make.png");
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
    }
    for (int i = 10; i < 20; ++i) {
      reset_image(i);
    }
  }
}
