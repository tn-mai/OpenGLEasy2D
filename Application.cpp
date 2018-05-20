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

// プレイヤーの勝ち回数.
int player_win_count = 0;

// CPUの勝ち回数.
int cpu_win_count = 0;

int player_hp_max;
int player_hp;
int player_mp_max;
int player_mp;
int player_attack;
int player_magic;
int player_defence;

int enemy_hp;
int enemy_mp;
int enemy_attack;
int enemy_magic;
int enemy_defence;

int player_progress;
bool next_place;

/**
* アプリケーションの本体.
*/
void application()
{
  // タイトル画面を表示
  if (title_flag) {
    title_flag = false;
    /*
    * この下にタイトル画面のプログラムを追加します.
    */
    set_image(0, 0, 0, "janken_background.png"); // 背景を表示.
    set_image(1, 0, 500, "janken_title.png");
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 0.5);
    scale_image(1, 0.5, 2.0, 0, 0);
    scale_image(1, 1, 1, 5, 2);
    move_image(1, 0, 50, 5, 2);
//    set_text(-100, -100, "Press Any Key");
    char buffer[20];
    select_string(-360, 0, 8, buffer);
    wait_any_key();

    fade_out(0, 0, 0, 1);
    reset_all_image();

    player_hp_max = 24;
    player_hp = player_hp_max;
    player_mp_max = 18;
    player_mp = player_mp_max;
    player_attack = 6;
    player_magic = 5;
    player_defence = 4;
    player_progress = 0;
    next_place = true;
  }

  /*
    洞窟に入ってゴブリンと3回戦い、最後にボスと戦う
    パラメータはHP、MP、攻撃力、魔法力、防御力
    選択肢は「攻撃、魔法、対魔法防御」

    魔法を使うにはMPが5点必要
    防御するにはMPが2点必要

    攻撃vs攻撃及び魔法vs魔法は双方のHPが減る
    防御vs防御は何も起こらない
    攻撃vs魔法は魔法の勝ち
    攻撃vs防御は攻撃の勝ち
    魔法vs防御は防御の勝ち

    攻撃によるダメージは(攻撃力-防御力)
    魔法は魔法力がそのままダメージとなる

    MPは戦闘終了時に最大5点回復する.

    画面配置
    +----------------------------------+
    |HP 12/20               敵HP  3/ 5 |
    |MP 10/15               敵MP  2/ 2 |
    |攻撃 6                            |
    |魔法 4         敵の               |
    |防御 3         画像               |
    |                                  |
    |どうする？                        |
    |戦う                              |
    |魔法(必要MP=5)                    |
    |対魔法防御(必要MP=2)              |
    +----------------------------------+
  */

  reset_all_text();
  set_text(-360, 260, "HP %d/%d", player_hp, player_hp_max);
  set_text(-360, 220, "MP %d/%d", player_mp, player_mp_max);
  set_text(-360, 180, "攻撃 %d", player_attack);
  set_text(-360, 140, "魔法 %d", player_magic);
  set_text(-360, 100, "防御 %d", player_defence);

  if (next_place) {
    if (player_progress == 0) {
      set_image(0, 0, 0, "cave00.jpg");
      set_image(1, 0, 0, "goblin.png");
      scale_image(1, 0.5, 0.5, 0, 0);
      enemy_hp = 7;
      enemy_mp = 2;
      enemy_attack = 5;
      enemy_magic = 2;
      enemy_defence = 2;
    } else if (player_progress == 1) {
      set_image(0, 0, 0, "cave01.jpg");
      set_image(1, 0, 0, "goblin.png");
      scale_image(1, 0.6, 0.6, 0, 0);
      color_blend_image(1, 0, 0, 0.2, 1, 1, 0, 0);
      enemy_hp = 10;
      enemy_mp = 5;
      enemy_attack = 5;
      enemy_magic = 4;
      enemy_defence = 3;
    } else if (player_progress == 2) {
      set_image(0, 0, 0, "cave02.jpg");
      set_image(1, 0, 0, "goblin.png");
      scale_image(1, 0.6, 0.6, 0, 0);
      color_blend_image(1, 0.2, 0, 0, 1, 1, 0, 0);
      enemy_hp = 12;
      enemy_mp = 9;
      enemy_attack = 6;
      enemy_magic = 4;
      enemy_defence = 3;
    } else {
      set_image(0, 0, 0, "Cave_26,_Ajanta.jpg");
      set_image(1, 0, 0, "demon.png");
      scale_image(1, 0.6, 0.6, 0, 0);
      color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
      color_blend_image(1, 1, 1, 1, 1, 0, 1, 2);
      enemy_hp = 20;
      enemy_mp = 14;
      enemy_attack = 7;
      enemy_magic = 8;
      enemy_defence = 4;
    }
    fade_in(1);
    const char* enemy_names[] = { "ゴブリン", "ゴブリンガード", "エルダーゴブリン", "デーモン" };
    set_text(-360, 0, "%sが現れた", enemy_names[player_progress]);
    next_place = false;
    player_progress += 1;
  }

  //  play_bgm("073.mp3");
  wait(1);

  // プレイヤーの行動選択.
  reset_text_area(-400, -299, 800, 300);
  set_text(-360, 0, "どうする？");
  int player_action = select(-360, -40, 3, "戦う", "魔法(必要MP=5)", "対魔法防御(必要MP=2)");
  if (player_action == 1) {
    if (player_mp < 5) {
      player_action = 0;
    } else {
      player_mp -= 5;
    }
  }
  if (player_action == 2) {
    if (player_mp < 2) {
      player_action = 0;
    } else {
      player_mp -= 2;
    }
  }

  // 敵の行動選択.
  int enemy_action = random(0, 2);
  if (enemy_action == 1) {
    if (enemy_mp < 5) {
      enemy_action = 0;
    } else {
      enemy_mp -= 5;
    }
  }
  if (enemy_action == 2) {
    if (enemy_mp < 2) {
      enemy_action = 0;
    } else {
      enemy_mp -= 2;
    }
  }

  reset_all_text();
  set_text(-360, 260, "HP %d/%d", player_hp, player_hp_max);
  set_text(-360, 220, "MP %d/%d", player_mp, player_mp_max);
  set_text(-360, 180, "攻撃 %d", player_attack);
  set_text(-360, 140, "魔法 %d", player_magic);
  set_text(-360, 100, "防御 %d", player_defence);

  if ((player_action != 2 && player_action == enemy_action) || (player_action == 0 && enemy_action == 2) || (player_action == 1 && enemy_action == 0) || (player_action == 2 && enemy_action == 1)) {
    reset_text_area(-400, -299, 800, 300);
    set_text(-360, 0, "あなたの攻撃！");
    wait(1);
    move_image(1, -10, 0, 1, 0.1);
    wait(0.1);
    move_image(1, 10, 0, 1, 0.1);
    wait(0.1);
    move_image(1, 0, 0, 1, 0.1);
    wait(0.5);
    if (player_action == 0) {
      enemy_hp -= player_attack - enemy_defence;
    } else if (player_action == 1) {
      enemy_hp -= player_magic;
    } else if (player_magic > enemy_magic) {
      enemy_hp -= player_magic - enemy_magic;
    }
    if (enemy_hp <= 0) {
      enemy_hp = 0;
      player_mp = player_mp_max;
      reset_text_area(-400, -299, 800, 300);
      set_text(-360, -0, "敵を倒した！");
      scale_image(1, 1.5, 0, 1, 1);
      move_image(1, 0, -100, 1, 1);
      next_place = true;
    }
  }
  if (enemy_hp > 0) {
    if ((player_action != 2 && player_action == enemy_action) || (player_action == 0 && enemy_action == 1) || (player_action == 1 && enemy_action == 2) || (player_action == 2 && enemy_action == 0)) {
      bool has_attack = false;
      if (enemy_action == 0) {
        player_hp -= enemy_attack - player_defence;
        has_attack = true;
      } else if (enemy_action == 1) {
        player_hp -= player_magic;
        has_attack = true;
      } else if (enemy_magic > player_magic) {
        player_hp -= enemy_magic - player_magic;
        has_attack = true;
      }
      reset_text_area(-400, -299, 800, 300);
      if (has_attack) {
        set_text(-360, 0, "敵の攻撃！");
        wait(1);
        move_image(0, 0, -10, 1, 0.1);
        wait(0.1);
        move_image(0, 0, 10, 1, 0.1);
        wait(0.1);
        move_image(0, 0, 0, 1, 0.1);
        wait(0.5);
      } else {
        set_text(-360, 0, "敵の攻撃をかわした！");
        wait(1);
      }
      if (player_hp <= 0) {
        player_hp = 0;
        reset_text_area(-400, -299, 800, 300);
        set_text(-360, 0, "あなたは死んでしまった…");
      }
    }
  }
  if (player_action == 2 && player_action == enemy_action) {
    reset_text_area(-400, -299, 800, 300);
    set_text(-360, 0, "互いに隙をうかがっている");
  }

  reset_text_area(-400, 1, 800, 300);
  set_text(-360, 260, "HP %d/%d", player_hp, player_hp_max);
  set_text(-360, 220, "MP %d/%d", player_mp, player_mp_max);
  set_text(-360, 180, "攻撃 %d", player_attack);
  set_text(-360, 140, "魔法 %d", player_magic);
  set_text(-360, 100, "防御 %d", player_defence);
  wait(1);

  if (enemy_hp <= 0) {
    if (player_progress <= 3) {
      set_text(-360, -40, "あなたは洞窟の奥へと進んだ");
      wait(1);
      fade_out(0, 0, 0, 1);
    } else {
      reset_text_area(-400, -299, 800, 300);
      set_text(-360, 0, "おめでとう！");
      set_text(-360, -40, "あなたはついにデーモンを倒した！");
      set_text(-360, -80, "世界は再び平和になるだろう。");
      set_text(-360, -120, "PUSH ANY KEY");
      wait_any_key();
      title_flag = true;
    }
  }

  if (player_hp <= 0) {
    set_text(-360, -40, "PUSH ANY KEY");
    wait_any_key();
    title_flag = true;
    wait(0.5);
  }
}