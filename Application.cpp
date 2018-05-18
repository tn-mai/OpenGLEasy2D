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

/**
* アプリケーションの本体.
*/
void application()
{
  set_image(0, 0, 0, "janken_background.png"); // 背景を表示.

  // タイトル画面を表示
  if (title_flag) {
    title_flag = false;
    /*
    * この下にタイトル画面のプログラムを追加します.
    */
#if 0
    color_blend_image(0, 0, 0, 0, 1, 0, 0, 0);
    set_image(1, 0, 0, "DeathCrimson_Monster.jpg");
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 2);
    wait(4);
    reset_image(1);
    set_text(-200, -200, "それは10年前のことであった...");
    wait(4);
    reset_all_text();
    set_text(-200, 20, "「ジャン=ケン」を取り返せ...");
    wait(2);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_EchizenWireFrame.jpg");
    wait(1);
    reset_image(1);
    set_text(-230, 20, "「ジャン=ケン」は奴が持っている");
    wait(2);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_EchizenWireFrame.jpg");
    wait(1);
    reset_image(1);
    set_text(-140, 20, "そいつの名前は...");
    wait(2);
    reset_all_text();
    set_text(-160, 200, "Full");
    wait(0.5);
    set_text(-100, 200, "Name");
    wait(0.5);
    set_text(0, 200, "越前");
    wait(0.5);
    set_text(100, 200, "康介");
    wait(0.5);
    set_text(-160, 100, "Code");
    wait(0.5);
    set_text(-100, 100, "Name");
    wait(0.5);
    set_text(0, 100, "コンバット");
    wait(0.5);
    set_text(150, 100, "越前");
    wait(0.5);
    set_text(-100, 0, "Age");
    wait(0.5);
    set_text(0, 0, "29");
    wait(0.5);
    set_text(-100, -100, "Size");
    wait(0.5);
    set_text(0, -100, "181cm");
    wait(0.5);
    set_text(100, -100, "70kg");
    wait(2);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_Echizen.jpg");
    wait(2);

    set_image(1, 0, 0, "DeathCrimson_Logo.jpg");
    move_image(1, -250, -250, 0, 1);
    rotate_image(1, 200, 0, 0);
    scale_image(1, 0.5, 0.5, 0, 0);
    scale_image(1, 1, 1, 0, 6);
    wait(1);
    move_image(1, 250, 250, 0, 1);
    rotate_image(1, 20, 0, 1);
    wait(1);
    move_image(1, -300, 0, 0, 1);
    wait(1);
    move_image(1, 300, -100, 0, 1);
    rotate_image(1, -360, 0, 3);
    wait(1);
    move_image(1, -300, 100, 0, 1);
    wait(1);
    move_image(1, 300, -50, 0, 1);
    wait(1);
    move_image(1, 100, 0, 0, 0.5);
    wait(0.5);
    move_image(1, 0, 50, 0, 0);
    wait(2);
    reset_image(1);

    set_text(-300, 200, "ダニー！グレッグ！生きてるか？");
    wait(2);
    set_text(-60, -100, "ああ、なんとかな！");
    wait(3);
    reset_all_text();
    set_text(-300, 200, "上からくるぞ！気をつけろよ！");
    wait(2);
    set_text(-60, -100, "こっちだ、越前！");
    wait(3);
    reset_all_text();
    set_text(-300, 200, "なんだこの階段は！？");
    wait(2);
    set_text(-60, -100, "とにかく、入ってみようぜ...");
    wait(3);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_RedDoor.jpg");
    scale_image(1, 1.25, 1.25, 0, 4.5);
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 2);
    wait(4);
    set_text(-300, -200, "せっかくだから、俺はこの赤の扉を選ぶぜ！");
    set_image(2, 0, 0, "DeathCrimson_RedDoorOpen.jpg");
    scale_image(2, 1.125, 1.125, 0, 0);
    color_blend_image(2, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(2, 1, 1, 1, 1, 0, 0, 4);
    wait(6);

    reset_image(1);
    reset_image(2);
    reset_all_text();
    set_text(-200, 20, "こうして越前は");
    set_text(-200, -20, "「ジャン=ケン」を手に入れた");
    wait(3);

    reset_all_text();
    set_text(-200, 40, "しかし今、");
    set_text(-200, 0, "デスビスノスの放った");
    set_text(-200, -40, "モンスターが");
    wait(3);

    reset_all_text();
    set_text(-200, 0, "越前に襲いかかる...");
    wait(4);

    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_Logo.jpg");
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 1);
    wait(1);
#else
    set_image(1, 0, 500, "janken_title.png");
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 0.5);
    scale_image(1, 0.5, 2.0, 0, 0);
    scale_image(1, 1, 1, 5, 2);
    move_image(1, 0, 50, 5, 2);
#endif
    set_text(-100, -100, "Press Any Key");
    wait_any_key();

    fade_out(0, 0, 0, 1);
    reset_all_text();
    reset_image(1);
    color_blend_image(0, 1, 1, 1, 1, 0, 0, 0);
    fade_in(1);
  }

  set_text(-360, 260, "あなた(%d勝)", player_win_count);
  set_text(40, 260, "CPU(%d勝)", cpu_win_count);

  // あいこフラグがtrueなら"あいこで"画像を表示. falseなら"じゃんけん"画像を表示.
  if (aiko_flag) {
    set_image(1, 0, 400, "janken_aikode.png");
  } else {
    set_image(1, 0, 400, "janken.png");
  }
  move_image(1, 0, 100, 1, 1.0f);
  play_bgm("073.mp3");
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
  scale_image(3, 1, 1, 4, 0.25f);

  play_sound("kotsudumi1.mp3");
  wait(2); // 2秒間待つ

  // 数値の比較は二重のイコール記号「==」で行う.
  // 「&&」は「且つ」、「||」は「又は」という意味を持つ記号.
  // 式の優先順位を調整するには算数と同様にカッコ「()」を使う.
  // グーは0、チョキは1、パーは2なので、例えばプレイヤーの手が0(グー)でCPUの手が1(チョキ)なら、プレイヤーの勝ちとなる.
  // 勝ったり負けたりした場合は「あいこ」じゃないのでaiko_flagを「偽」にしておく.
  // 勝ってもいないし負けてもいない場合は「あいこ」なのでaiko_flagを「真」にしておく.
  if ((player_hand == 0 && cpu_hand == 1) || (player_hand == 1 && cpu_hand == 2) || (player_hand == 2 && cpu_hand == 0)) {
    play_sound("correct4.mp3");
    set_image(4, 0, -150, "janken_kachi.png");
    player_win_count += 1; // プレイヤーの勝利数を1増やす.
    aiko_flag = false;
  } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
    play_sound("incorrect1.mp3");
    set_image(4, 0, -150, "janken_make.png");
    cpu_win_count += 1; // CPUの勝利数を1増やす.
    aiko_flag = false;
  } else {
    play_sound("stupid2.mp3");
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
    play_sound("switch1.mp3");
    // "やめる"が選ばれたらアプリケーションを終了する.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // アプリケーションを終了させる.
    }
  }
  reset_all_image(); // 次の勝負に備えてすべての画像を消す.
  reset_all_text(); // 次の勝負に備えてすべての文字を消す.
}