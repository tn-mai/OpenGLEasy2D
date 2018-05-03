/**
* @file Application.cpp
*/
#include "Command.h"

// 前回「あいこ」のときtrue、そうでなければfalseが格納される.
// trueは「真」、falseは「偽」という意味.
// ひとつのイコール記号は代入を意味する.
bool aiko_flag = false;

// プレイヤーの勝ち回数.
int player_win_count = 0;

// CPUの勝ち回数.
int cpu_win_count = 0;

/**
* アプリケーションの本体.
*/
void application()
{
  set_image(0, 0, 0, "janken_background.png"); // 背景を表示.

  set_text(-360, 260, "あなた(%d勝)", player_win_count);
  set_text(40, 260, "CPU(%d勝)", cpu_win_count);

  // あいこフラグがtrueなら"あいこで"画像を表示. falseなら"じゃんけん"画像を表示.
  if (aiko_flag) {
    set_image(1, 0, 400, "janken_aikode.png");
  } else {
    set_image(1, 0, 400, "janken.png");
  }
  move_image(1, 0, 100, 1, 1.0f);
  play_bgm("Res/7050.mp3");
  wait(1);

  set_text(-360, 0, "手を選んでください");

  // プレイヤーの手を選んでもらう.
  // 選ばれたのがグーなら0が、チョキなら1が、パーなら2がplayer_handに格納される.
  const int player_hand = select(-360, -40, 3, "グー", "チョキ", "パー");

  // CPUの手を選ぶ.
  // 選んだのがグーなら0が、チョキなら1が、パーなら2がcpu_handに格納される.
  const int cpu_hand = random(0, 2); // 0～2のいずれかの整数が無作為に選ばれる.

  reset_image(1); // "じゃんけん"(または"あいこで")画像を消す.

  // "ほい!"

  // 左側にプレイヤーの手を表示
  switch (player_hand) {
  case 0: set_image(2, -200, 100, "janken_gu.png"); break;
  case 1: set_image(2, -200, 100, "janken_choki.png"); break;
  case 2: set_image(2, -200, 100, "janken_pa.png"); break;
  }
  scale_image(2, 0, 0, 0, 0);
  scale_image(2, 1, 1, 4, 0.25f);

  // 右側にCPUの手を表示
  switch (cpu_hand) {
  case 0: set_image(3, 200, 100, "janken_gu.png"); break;
  case 1: set_image(3, 200, 100, "janken_choki.png"); break;
  case 2: set_image(3, 200, 100, "janken_pa.png"); break;
  }
  scale_image(3, 0, 0, 0, 0);
  scale_image(3, 1, 1, 5, 0.25f);

  wait(2); // 2秒間待つ

  // 数値の比較は二重のイコール記号「==」で行う.
  // 「&&」は「且つ」、「||」は「又は」という意味を持つ記号.
  // 式の優先順位を調整するには算数と同様にカッコ「()」を使う.
  // グーは0、チョキは1、パーは2なので、例えばプレイヤーの手が0(グー)でCPUの手が1(チョキ)なら、プレイヤーの勝ちとなる.
  // 勝ったり負けたりした場合は「あいこ」じゃないのでaiko_flagを「偽」にしておく.
  // 勝ってもいないし負けてもいない場合は「あいこ」なのでaiko_flagを「真」にしておく.
  if ((player_hand == 0 && cpu_hand == 1) || (player_hand == 1 && cpu_hand == 2) || (player_hand == 2 && cpu_hand == 0)) {
    set_image(4, 0, -150, "janken_kachi.png");
    player_win_count += 1; // プレイヤーの勝利数を1増やす.
    aiko_flag = false;
  } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
    set_image(4, 0, -150, "janken_make.png");
    cpu_win_count += 1; // CPUの勝利数を1増やす.
    aiko_flag = false;
  } else {
    aiko_flag = true;
  }

  reset_all_text(); // いったんすべての文字を消す.

  // 勝利回数が変化した可能性があるので表示しなおす.
  set_text(-360, 260, "あなた(%d勝)", player_win_count);
  set_text(40, 260, "CPU(%d勝)", cpu_win_count);

  // 「あいこ」だった場合は自動的に勝負を継続する.
  if (aiko_flag == false) {
    wait(3); // 3秒間待つ.
    set_text(-360, -40, "勝負を続けますか？");
    const int yes_or_no = select(-360, -80, 2, "続ける", "やめる");
    // "やめる"が選ばれたらアプリケーションを終了する.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // アプリケーションを終了させる.
    }
  }
  reset_all_image(); // 次の勝負に備えてすべての画像を消す.
  reset_all_text(); // 次の勝負に備えてすべての文字を消す.
}