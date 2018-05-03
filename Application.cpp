/**
* @file Application.cpp
*/
#include "Command.h"

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

  set_text(-360, 260, "���Ȃ�(%d��)", player_win_count);
  set_text(40, 260, "CPU(%d��)", cpu_win_count);

  // �������t���O��true�Ȃ�"��������"�摜��\��. false�Ȃ�"����񂯂�"�摜��\��.
  if (aiko_flag) {
    set_image(1, 0, 400, "janken_aikode.png");
  } else {
    set_image(1, 0, 400, "janken.png");
  }
  move_image(1, 0, 100, 1, 1.0f);
  play_bgm("Res/7050.mp3");
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
  scale_image(3, 1, 1, 5, 0.25f);

  wait(2); // 2�b�ԑ҂�

  // ���l�̔�r�͓�d�̃C�R�[���L���u==�v�ōs��.
  // �u&&�v�́u���v�A�u||�v�́u���́v�Ƃ����Ӗ������L��.
  // ���̗D�揇�ʂ𒲐�����ɂ͎Z���Ɠ��l�ɃJ�b�R�u()�v���g��.
  // �O�[��0�A�`���L��1�A�p�[��2�Ȃ̂ŁA�Ⴆ�΃v���C���[�̎肪0(�O�[)��CPU�̎肪1(�`���L)�Ȃ�A�v���C���[�̏����ƂȂ�.
  // �������蕉�����肵���ꍇ�́u�������v����Ȃ��̂�aiko_flag���u�U�v�ɂ��Ă���.
  // �����Ă����Ȃ��������Ă����Ȃ��ꍇ�́u�������v�Ȃ̂�aiko_flag���u�^�v�ɂ��Ă���.
  if ((player_hand == 0 && cpu_hand == 1) || (player_hand == 1 && cpu_hand == 2) || (player_hand == 2 && cpu_hand == 0)) {
    set_image(4, 0, -150, "janken_kachi.png");
    player_win_count += 1; // �v���C���[�̏�������1���₷.
    aiko_flag = false;
  } else if ((player_hand == 0 && cpu_hand == 2) || (player_hand == 1 && cpu_hand == 0) || (player_hand == 2 && cpu_hand == 1)) {
    set_image(4, 0, -150, "janken_make.png");
    cpu_win_count += 1; // CPU�̏�������1���₷.
    aiko_flag = false;
  } else {
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
    // "��߂�"���I�΂ꂽ��A�v���P�[�V�������I������.
    if (yes_or_no == 1) {
      fade_out(0, 0, 0, 1);
      quit(); // �A�v���P�[�V�������I��������.
    }
  }
  reset_all_image(); // ���̏����ɔ����Ă��ׂẲ摜������.
  reset_all_text(); // ���̏����ɔ����Ă��ׂĂ̕���������.
}