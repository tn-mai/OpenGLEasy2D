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

// �v���C���[�̌��݈ʒu.
int player_x;
int player_y;

// �v���C���[�̌���.
const int dir_up = 0;
const int dir_right = 1;
const int dir_down = 2;
const int dir_left = 3;
int player_direction;

// �v���C���[�̍ő�q�b�g�|�C���g.
const int player_hp_max = 10;

// �v���C���[�̃q�b�g�|�C���g.
int player_hp;

// �G�Ƃ̑����m��.
const int encount_percent = 20;

// �_���W�����̑傫��
const int dungeon_width = 8;
const int dungeon_height = 8;

/**
* �_���W�����}�b�v.
*
* 0: �ʘH
* 1: ��
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

// �X�^�[�g�̈ʒu.
const int start_x = 1;
const int start_y = 1;

// �S�[���̈ʒu.
const int goal_x = 3;
const int goal_y = 1;

// �퓬���̂Ƃ���true. �����łȂ����false.
bool battle_flag;

/**
* �A�v���P�[�V�����̖{��.
*/
void application()
{
  // �^�C�g����ʂ�\��
  if (title_flag) {
    /*
    * �^�C�g�����.
    */
    reset_all_image();
    reset_all_text();
    set_image(0, 0, 0, "janken_background.png"); // �w�i��\��.
    set_image(1, 0, 500, "janken_title.png");
    move_image(1, 0, 50, 5, 1.5);
    scale_image(1, 0.5, 2, 0, 0);
    scale_image(1, 1, 1, 5, 1.5);
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 0.5);
    wait(2);
    const int yes_or_no = select(-80, -80, 2, "�͂��߂�", "��߂�");
    play_sound("switch1.mp3");
    // "��߂�"���I�΂ꂽ��A�v���P�[�V�������I������.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // �A�v���P�[�V�������I��������.
    } else {
      title_flag = false;
      fade_out(0, 0, 0, 1);
      reset_all_text();
      reset_all_image();
      play_bgm("073.mp3");
      fade_in(1);

      // �Q�[���̏����ݒ���s��.
      player_x = start_x;
      player_y = start_y;
      player_direction = dir_down;
      player_hp = player_hp_max;
      aiko_flag = false;
      battle_flag = false;
    }
  } else if (!battle_flag) {
    /*
    * �_���W�����T��.
    */
    reset_all_image();
    reset_all_text();

    // �����ɉ����ĕǂ̗L���𒲂ׂ�.
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

    // �ǂ̂��镔���ɉ摜��z�u.
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

    // ���݈ʒu�ƌ�����\��.
    const char* const direction_text[] = { "�k", "��", "��", "��" };
    set_text(-360, 260, "(%d, %d) %s", player_x, player_y, direction_text[player_direction]);

    // �m���œG�Ƒ���.
    if (random(0, 99) < encount_percent) {
      set_text(-360, 0, "�����Ɍ��������I");
      wait(2);
      battle_flag = true;
    } else {
      if (player_x == goal_x && player_y == goal_y) {
        // �S�[���ɓ��B�����̂Ń��b�Z�[�W��\�����ă^�C�g���ɖ߂�.
        set_text(-360, 0, "�o�����I");
        set_text(-360, -40, "���߂łƂ��I");
        set_text(-360, -80, "���Ȃ��̓_���W��������̒E�o�ɐ��������I");
        set_text(-360, -120, "�f�`�l�d�@�b�k�d�`�q");
        wait(1);
        set_text(-360, -160, "(�����L�[�������ƃ^�C�g���ɖ߂�܂�)");
        title_flag = true;
        wait_any_key();
        wait(1);
      } else {
        // �S�[���ł͂Ȃ��̂ōs����I������.
        set_text(-360, 0, "�ǂ����܂����H");
        const int player_action = select(-360, -40, 4, "�O�i", "�E������", "��������", "��������");
        if (player_action == 0) {
          if (wall_front[0]) {
            // �O���ɕǂ�����ꍇ�͐i�߂Ȃ�.
            reset_text_area(-400, -200, 800, 301);
            set_text(-360, 0, "�ǂ������Đi�߂Ȃ�");
            wait(1.5);
          } else {
            // �ǂ��Ȃ���΁A�v���C���[�̑O���ֈړ�����.
            const int move[4][2] = {{ 0, -1 },{ 1, 0 },{ 0, 1 },{ -1, 0 }};
            player_x += move[player_direction][0];
            player_y += move[player_direction][1];
          }
        } else {
          // �v���C���[�̌�����ς���.
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
    * ����񂯂�o�g��.
    */
    reset_all_text();
    set_text(-360, 260, "���Ȃ�(HP %d/%d)", player_hp, player_hp_max);
    set_image(10, 0, -100, "goblin.png");
    scale_image(10, 0.5, 0.5, 0, 0);

    // �������t���O��true�Ȃ�"��������"�摜��\��. false�Ȃ�"����񂯂�"�摜��\��.
    const char* aiko_images[] = { "janken.png", "janken_aikode.png" };
    set_image(11, 0, 400, aiko_images[aiko_flag]);
    move_image(11, 0, 100, 4, 0.75);
    wait(0.75);

    set_text(-360, 0, "���I��ł�������");

    // �v���C���[�̎��I��ł��炤.
    // �I�΂ꂽ�̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��player_hand�Ɋi�[�����.
    const int player_hand = select(-360, -40, 3, "�O�[", "�`���L", "�p�[");

    // CPU�̎��I��.
    // �I�񂾂̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��cpu_hand�Ɋi�[�����.
    const int cpu_hand = random(0, 2); // 0�`2�̂����ꂩ�̐���������ׂɑI�΂��.

    reset_image(11); // "����񂯂�"(�܂���"��������")�摜������.

    const char* janken_images[] = { "janken_gu.png", "janken_choki.png", "janken_pa.png" };
    // �����Ƀv���C���[�̎��\��
    set_image(12, -200, 100, janken_images[player_hand]);
    scale_image(12, 0, 0, 0, 0);
    scale_image(12, 1, 1, 4, 0.25f);

    // �E����CPU�̎��\��
    set_image(13, 200, 100, janken_images[cpu_hand]);
    scale_image(13, 0, 0, 0, 0);
    scale_image(13, 1, 1, 4, 0.25f);

    play_sound("kotsudumi1.mp3");
    wait(2); // 2�b�ԑ҂�

    // ���l�̔�r�͓�d�̃C�R�[���L���u==�v�ōs��.
    // �u&&�v�́u���v�A�u||�v�́u���́v�Ƃ����Ӗ������L��.
    // ���̗D�揇�ʂ𒲐�����ɂ͎Z���Ɠ��l�ɃJ�b�R�u()�v���g��.
    // �O�[��0�A�`���L��1�A�p�[��2�Ȃ̂ŁA�Ⴆ�΃v���C���[�̎肪0(�O�[)��CPU�̎肪1(�`���L)�Ȃ�A�v���C���[�̏����ƂȂ�.
    // �������蕉�����肵���ꍇ�́u�������v����Ȃ��̂�aiko_flag���u�U�v�ɂ��Ă���.
    // �����Ă����Ȃ��������Ă����Ȃ��ꍇ�́u�������v�Ȃ̂�aiko_flag���u�^�v�ɂ��Ă���.
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

    reset_all_text(); // �������񂷂ׂĂ̕���������.

    // �q�b�g�|�C���g���ω������\��������̂ŕ\�����Ȃ���.
    set_text(-360, 260, "���Ȃ�(HP %d/%d)", player_hp, player_hp_max);

    if (win_flag) {
      battle_flag = false;
      set_text(-360, 0, "�����ɏ������I");
      wait(2);
    } else if (player_hp <= 0) {
      battle_flag = false;
      set_text(-360, 0, "���Ȃ��͉����ɂ���Ă��܂����c");
      set_text(-360, -40, "�f�`�l�d�@�n�u�d�q");
      wait(1);
      set_text(-360, -80, "(�����L�[�������ƃ^�C�g���ɖ߂�܂�)");
      wait_any_key();
      wait(1);
      title_flag = true;
    }
    for (int i = 10; i < 20; ++i) {
      reset_image(i);
    }
  }
}
