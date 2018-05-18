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

/**
* �A�v���P�[�V�����̖{��.
*/
void application()
{
  set_image(0, 0, 0, "janken_background.png"); // �w�i��\��.

  // �^�C�g����ʂ�\��
  if (title_flag) {
    title_flag = false;
    /*
    * ���̉��Ƀ^�C�g����ʂ̃v���O������ǉ����܂�.
    */
#if 0
    color_blend_image(0, 0, 0, 0, 1, 0, 0, 0);
    set_image(1, 0, 0, "DeathCrimson_Monster.jpg");
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 2);
    wait(4);
    reset_image(1);
    set_text(-200, -200, "�����10�N�O�̂��Ƃł�����...");
    wait(4);
    reset_all_text();
    set_text(-200, 20, "�u�W����=�P���v�����Ԃ�...");
    wait(2);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_EchizenWireFrame.jpg");
    wait(1);
    reset_image(1);
    set_text(-230, 20, "�u�W����=�P���v�͓z�������Ă���");
    wait(2);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_EchizenWireFrame.jpg");
    wait(1);
    reset_image(1);
    set_text(-140, 20, "�����̖��O��...");
    wait(2);
    reset_all_text();
    set_text(-160, 200, "Full");
    wait(0.5);
    set_text(-100, 200, "Name");
    wait(0.5);
    set_text(0, 200, "�z�O");
    wait(0.5);
    set_text(100, 200, "�N��");
    wait(0.5);
    set_text(-160, 100, "Code");
    wait(0.5);
    set_text(-100, 100, "Name");
    wait(0.5);
    set_text(0, 100, "�R���o�b�g");
    wait(0.5);
    set_text(150, 100, "�z�O");
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

    set_text(-300, 200, "�_�j�[�I�O���b�O�I�����Ă邩�H");
    wait(2);
    set_text(-60, -100, "�����A�Ȃ�Ƃ��ȁI");
    wait(3);
    reset_all_text();
    set_text(-300, 200, "�ォ�炭�邼�I�C�������I");
    wait(2);
    set_text(-60, -100, "���������A�z�O�I");
    wait(3);
    reset_all_text();
    set_text(-300, 200, "�Ȃ񂾂��̊K�i�́I�H");
    wait(2);
    set_text(-60, -100, "�Ƃɂ����A�����Ă݂悤��...");
    wait(3);
    reset_all_text();
    set_image(1, 0, 0, "DeathCrimson_RedDoor.jpg");
    scale_image(1, 1.25, 1.25, 0, 4.5);
    color_blend_image(1, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(1, 1, 1, 1, 1, 0, 0, 2);
    wait(4);
    set_text(-300, -200, "��������������A���͂��̐Ԃ̔���I�Ԃ��I");
    set_image(2, 0, 0, "DeathCrimson_RedDoorOpen.jpg");
    scale_image(2, 1.125, 1.125, 0, 0);
    color_blend_image(2, 1, 1, 1, 0, 0, 0, 0);
    color_blend_image(2, 1, 1, 1, 1, 0, 0, 4);
    wait(6);

    reset_image(1);
    reset_image(2);
    reset_all_text();
    set_text(-200, 20, "�������ĉz�O��");
    set_text(-200, -20, "�u�W����=�P���v����ɓ��ꂽ");
    wait(3);

    reset_all_text();
    set_text(-200, 40, "���������A");
    set_text(-200, 0, "�f�X�r�X�m�X�̕�����");
    set_text(-200, -40, "�����X�^�[��");
    wait(3);

    reset_all_text();
    set_text(-200, 0, "�z�O�ɏP��������...");
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

  set_text(-360, 260, "���Ȃ�(%d��)", player_win_count);
  set_text(40, 260, "CPU(%d��)", cpu_win_count);

  // �������t���O��true�Ȃ�"��������"�摜��\��. false�Ȃ�"����񂯂�"�摜��\��.
  if (aiko_flag) {
    set_image(1, 0, 400, "janken_aikode.png");
  } else {
    set_image(1, 0, 400, "janken.png");
  }
  move_image(1, 0, 100, 1, 1.0f);
  play_bgm("073.mp3");
  wait(1);

  set_text(-360, 0, "���I��ł�������");

  // �v���C���[�̎��I��ł��炤.
  // �I�΂ꂽ�̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��player_hand�Ɋi�[�����.
  const int player_hand = select(-360, -40, 3, "�O�[", "�`���L", "�p�[");

  // CPU�̎��I��.
  // �I�񂾂̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��cpu_hand�Ɋi�[�����.
  const int cpu_hand = random(0, 2); // 0�`2�̂����ꂩ�̐���������ׂɑI�΂��.

  reset_image(1); // "����񂯂�"(�܂���"��������")�摜������.

  // "�ق�!"

  // �����Ƀv���C���[�̎��\��
  switch (player_hand) {
  case 0: set_image(2, -200, 100, "janken_gu.png"); break;
  case 1: set_image(2, -200, 100, "janken_choki.png"); break;
  case 2: set_image(2, -200, 100, "janken_pa.png"); break;
  }
  scale_image(2, 0, 0, 0, 0);
  scale_image(2, 1, 1, 4, 0.25f);

  // �E����CPU�̎��\��
  switch (cpu_hand) {
  case 0: set_image(3, 200, 100, "janken_gu.png"); break;
  case 1: set_image(3, 200, 100, "janken_choki.png"); break;
  case 2: set_image(3, 200, 100, "janken_pa.png"); break;
  }
  scale_image(3, 0, 0, 0, 0);
  scale_image(3, 1, 1, 4, 0.25f);

  play_sound("kotsudumi1.mp3");
  wait(2); // 2�b�ԑ҂�

  // ���l�̔�r�͓�d�̃C�R�[���L���u==�v�ōs��.
  // �u&&�v�́u���v�A�u||�v�́u���́v�Ƃ����Ӗ������L��.
  // ���̗D�揇�ʂ𒲐�����ɂ͎Z���Ɠ��l�ɃJ�b�R�u()�v���g��.
  // �O�[��0�A�`���L��1�A�p�[��2�Ȃ̂ŁA�Ⴆ�΃v���C���[�̎肪0(�O�[)��CPU�̎肪1(�`���L)�Ȃ�A�v���C���[�̏����ƂȂ�.
  // �������蕉�����肵���ꍇ�́u�������v����Ȃ��̂�aiko_flag���u�U�v�ɂ��Ă���.
  // �����Ă����Ȃ��������Ă����Ȃ��ꍇ�́u�������v�Ȃ̂�aiko_flag���u�^�v�ɂ��Ă���.
  if ((player_hand == 0 && cpu_hand == 1) || (player_hand == 1 && cpu_hand == 2) || (player_hand == 2 && cpu_hand == 0)) {
    play_sound("correct4.mp3");
    set_image(4, 0, -150, "janken_kachi.png");
    player_win_count += 1; // �v���C���[�̏�������1���₷.
    aiko_flag = false;
  } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
    play_sound("incorrect1.mp3");
    set_image(4, 0, -150, "janken_make.png");
    cpu_win_count += 1; // CPU�̏�������1���₷.
    aiko_flag = false;
  } else {
    play_sound("stupid2.mp3");
    aiko_flag = true;
  }

  reset_all_text(); // �������񂷂ׂĂ̕���������.

  // �����񐔂��ω������\��������̂ŕ\�����Ȃ���.
  set_text(-360, 260, "���Ȃ�(%d��)", player_win_count);
  set_text(40, 260, "CPU(%d��)", cpu_win_count);

  // �u�������v�������ꍇ�͎����I�ɏ������p������.
  if (aiko_flag == false) {
    wait(3); // 3�b�ԑ҂�.
    set_text(-360, -40, "�����𑱂��܂����H");
    const int yes_or_no = select(-360, -80, 2, "������", "��߂�");
    play_sound("switch1.mp3");
    // "��߂�"���I�΂ꂽ��A�v���P�[�V�������I������.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // �A�v���P�[�V�������I��������.
    }
  }
  reset_all_image(); // ���̏����ɔ����Ă��ׂẲ摜������.
  reset_all_text(); // ���̏����ɔ����Ă��ׂĂ̕���������.
}