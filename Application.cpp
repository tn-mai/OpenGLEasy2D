/**
* @file Application.cpp
*/
#include "Command.h"

// ��x�^�C�g����ʂ�\�������false�ɂȂ�.
bool title_flag = true;

// �O��u�������v�̂Ƃ�true�A�����łȂ����false���i�[�����.
// true�́u�^�v�Afalse�́u�U�v�Ƃ����Ӗ�.
// �ЂƂ̃C�R�[���L���͑�����Ӗ�����.
bool aiko_flag = false;

// �v���C���[�̏�����.
int player_win_count = 0;

// CPU�̏�����.
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
* �A�v���P�[�V�����̖{��.
*/
void application()
{
  // �^�C�g����ʂ�\��
  if (title_flag) {
    title_flag = false;
    /*
    * ���̉��Ƀ^�C�g����ʂ̃v���O������ǉ����܂�.
    */
    set_image(0, 0, 0, "janken_background.png"); // �w�i��\��.
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
    ���A�ɓ����ăS�u������3��킢�A�Ō�Ƀ{�X�Ɛ키
    �p�����[�^��HP�AMP�A�U���́A���@�́A�h���
    �I�����́u�U���A���@�A�Ζ��@�h��v

    ���@���g���ɂ�MP��5�_�K�v
    �h�䂷��ɂ�MP��2�_�K�v

    �U��vs�U���y�і��@vs���@�͑o����HP������
    �h��vs�h��͉����N����Ȃ�
    �U��vs���@�͖��@�̏���
    �U��vs�h��͍U���̏���
    ���@vs�h��͖h��̏���

    �U���ɂ��_���[�W��(�U����-�h���)
    ���@�͖��@�͂����̂܂܃_���[�W�ƂȂ�

    MP�͐퓬�I�����ɍő�5�_�񕜂���.

    ��ʔz�u
    +----------------------------------+
    |HP 12/20               �GHP  3/ 5 |
    |MP 10/15               �GMP  2/ 2 |
    |�U�� 6                            |
    |���@ 4         �G��               |
    |�h�� 3         �摜               |
    |                                  |
    |�ǂ�����H                        |
    |�키                              |
    |���@(�K�vMP=5)                    |
    |�Ζ��@�h��(�K�vMP=2)              |
    +----------------------------------+
  */

  reset_all_text();
  set_text(-360, 260, "HP %d/%d", player_hp, player_hp_max);
  set_text(-360, 220, "MP %d/%d", player_mp, player_mp_max);
  set_text(-360, 180, "�U�� %d", player_attack);
  set_text(-360, 140, "���@ %d", player_magic);
  set_text(-360, 100, "�h�� %d", player_defence);

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
    const char* enemy_names[] = { "�S�u����", "�S�u�����K�[�h", "�G���_�[�S�u����", "�f�[����" };
    set_text(-360, 0, "%s�����ꂽ", enemy_names[player_progress]);
    next_place = false;
    player_progress += 1;
  }

  //  play_bgm("073.mp3");
  wait(1);

  // �v���C���[�̍s���I��.
  reset_text_area(-400, -299, 800, 300);
  set_text(-360, 0, "�ǂ�����H");
  int player_action = select(-360, -40, 3, "�키", "���@(�K�vMP=5)", "�Ζ��@�h��(�K�vMP=2)");
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

  // �G�̍s���I��.
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
  set_text(-360, 180, "�U�� %d", player_attack);
  set_text(-360, 140, "���@ %d", player_magic);
  set_text(-360, 100, "�h�� %d", player_defence);

  if ((player_action != 2 && player_action == enemy_action) || (player_action == 0 && enemy_action == 2) || (player_action == 1 && enemy_action == 0) || (player_action == 2 && enemy_action == 1)) {
    reset_text_area(-400, -299, 800, 300);
    set_text(-360, 0, "���Ȃ��̍U���I");
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
      set_text(-360, -0, "�G��|�����I");
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
        set_text(-360, 0, "�G�̍U���I");
        wait(1);
        move_image(0, 0, -10, 1, 0.1);
        wait(0.1);
        move_image(0, 0, 10, 1, 0.1);
        wait(0.1);
        move_image(0, 0, 0, 1, 0.1);
        wait(0.5);
      } else {
        set_text(-360, 0, "�G�̍U�������킵���I");
        wait(1);
      }
      if (player_hp <= 0) {
        player_hp = 0;
        reset_text_area(-400, -299, 800, 300);
        set_text(-360, 0, "���Ȃ��͎���ł��܂����c");
      }
    }
  }
  if (player_action == 2 && player_action == enemy_action) {
    reset_text_area(-400, -299, 800, 300);
    set_text(-360, 0, "�݂��Ɍ������������Ă���");
  }

  reset_text_area(-400, 1, 800, 300);
  set_text(-360, 260, "HP %d/%d", player_hp, player_hp_max);
  set_text(-360, 220, "MP %d/%d", player_mp, player_mp_max);
  set_text(-360, 180, "�U�� %d", player_attack);
  set_text(-360, 140, "���@ %d", player_magic);
  set_text(-360, 100, "�h�� %d", player_defence);
  wait(1);

  if (enemy_hp <= 0) {
    if (player_progress <= 3) {
      set_text(-360, -40, "���Ȃ��͓��A�̉��ւƐi��");
      wait(1);
      fade_out(0, 0, 0, 1);
    } else {
      reset_text_area(-400, -299, 800, 300);
      set_text(-360, 0, "���߂łƂ��I");
      set_text(-360, -40, "���Ȃ��͂��Ƀf�[������|�����I");
      set_text(-360, -80, "���E�͍Ăѕ��a�ɂȂ邾�낤�B");
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