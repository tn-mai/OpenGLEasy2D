#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

/**
* ������\������.
*
* @param  x      �\���J�n�ʒu�̍��[���W.
* @param  y      �\���J�n�ʒu�̏�[���W.
* @param  format �����̏����w��(printf�֐��̂��̂Ɠ���).
* @param  ...    �ǉ��̈���(printf�֐��̂��̂Ɠ���).
*
* �\���J�n�ʒu�̓E�B���h�E�̒��S�����_�Ƃ��A���E��X��(�E���v���X����)�A�㉺��Y��(�オ�v���X����)�̍��W�n�Ŏw�肷��.
* �E�B���h�E�̑傫���͉�800�h�b�g�A�c600�h�b�g�ł���.
*/
void set_text(float x, float y, const char* format, ...);

/**
* ���ׂĂ̕���������.
*/
void reset_all_text();

/**
* �摜��z�u����.
*
* @param  no       �摜�̊Ǘ��ԍ�.
* @param  x        �\���ʒu(X���W).
* @param  y        �\���ʒu(Y���W).
* @param  filename �摜�t�@�C���̖��O.
*
* ���̃v���O�����ł́A�z�u�����摜���ƂɊǗ��ԍ���t����.
* �摜�̈ړ�������͊Ǘ��ԍ���ʂ��čs��.
*
* �\���ʒu�̓E�B���h�E�̒��S�����_�Ƃ��A���E��X��(�E���v���X����)�A�㉺��Y��(�オ�v���X����)�̍��W�n�Ŏw�肷��.
* �E�B���h�E�̑傫���͉�800�h�b�g�A�c600�h�b�g�ł���.
* �����Ŏw�肷����W�͉摜�̒��S���w��.
*
* no�Ŏw�肵���Ǘ��ԍ��Ɋ��ɕʂ̉摜��z�u���Ă����ꍇ�A
* �܂����̉摜���������Ă���A���߂�filename�Ŏw�肵���摜��z�u����.
*/
void set_image(int no, float x, float y, const char* filename);

/**
* �摜���ړ�����.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  x       �ړ����X���W.
* @param  y       �ړ����Y���W.
* @param  easing  ��ԕ��@:
*                   0 �����ňړ�.
*                   1 �������Ȃ���ړ�.
*                   2 �������Ȃ���ړ�.
*                   3 �������Ȃ���ړ����n�߂āA�������Ȃ����~����.
*                   4 1�Ɏ��Ă��邪�A�����s���߂��Ă���߂��Ă��Ē�~����.
*                   5 1�Ɏ��Ă��邪�A�ړ�����W�ŉ��x���e��ł����~����.
* @param  seconds ���쎞��(�b).
*
* �ʒu�̓E�B���h�E�̒��S�����_�Ƃ��A���E��X��(�E���v���X����)�A�㉺��Y��(�オ�v���X����)�̍��W�n�Ŏw�肷��.
* �E�B���h�E�̑傫���͉�800�h�b�g�A�c600�h�b�g�ł���.
* �����Ŏw�肷����W�͉摜�̒��S���w��.
*/
void move_image(int no, float x, float y, int easing, float seconds);

/**
* �摜���g��E�k������.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  x       �g��E�k�����X�����̑傫��. �}�C�i�X�l��ݒ肷��ƍ��E�����]����.
* @param  y       �g��E�k�����Y�����̑傫��. �}�C�i�X�l��ݒ肷��Ə㉺�����]����.
* @param  easing  ��ԕ��@:
*                   0 �����Ŋg��E�k��.
*                   1 �������Ȃ���g��E�k��.
*                   2 �������Ȃ���g��E�k��.
*                   3 �������Ȃ���g��E�k�����n�߂āA�������Ȃ����~����.
*                   4 1�Ɏ��Ă��邪�A�����g��E�k�����߂��Ă���߂��Ă��Ē�~����.
*                   5 1�Ɏ��Ă��邪�A���x���e�ނ悤�Ɋg��E�k�����Ȃ����~����.
* @param  seconds ���쎞��(�b).
*/
void scale_image(int no, float x, float y, int easing, float seconds);

/**
* �摜����]����.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  degree  ��]������p�x(�x���@). �}�C�i�X�l��ݒ肷��Ɖ�]�������t�ɂȂ�.
* @param  easing  ��ԕ��@:
*                   0 �����ŉ�].
*                   1 �������Ȃ����].
*                   2 �������Ȃ����].
*                   3 �������Ȃ����]���n�߂āA�������Ȃ����~����.
*                   4 1�Ɏ��Ă��邪�A������]���߂��Ă���߂��Ă��Ē�~����.
*                   5 1�Ɏ��Ă��邪�A���x���e�݂Ȃ����~����.
* @param  seconds ���쎞��(�b).
*/
void rotate_image(int no, float degree, int easing, float seconds);

/**
* �摜�ƐF����������.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  red     ��������F�̐Ԑ���(0.0�`1.0).
* @param  green   ��������F�̗ΐ���(0.0�`1.0).
* @param  blue    ��������F�̐���(0.0�`1.0).
* @param  alpha   ��������F�̓����x(0.0�`1.0).
* @param  mod     �������@:
*                   0 ��Z
*                   1 ���Z
*                   2 ���Z
* @param  easing  ��ԕ��@:
*                   0 �����ŐF������.
*                   1 �������Ȃ���F������.
*                   2 �������Ȃ���F������.
*                   3 �������Ȃ��獇�����n�߂āA�������Ȃ����~����.
*                   4 1�Ɏ��Ă��邪�A�����F���o���߂��Ă���߂��Ă��Ē�~����.
*                   5 1�Ɏ��Ă��邪�A���x���e�ނ悤�ɍ������Ȃ����~����.
* @param  seconds ���쎞��(�b).
*/
void color_blend_image(int no, float red, float green, float blue, float alpha, int mode, int easing, float seconds);

/**
* ���ׂẲ摜������.
*/
void reset_all_image();

/**
* �Ǘ��ԍ��Ŏw�肳�ꂽ�摜������.
*
* @param  no �摜�̊Ǘ��ԍ�.
*
* �Ώۂ̉摜�����łɏ�����Ă����ꍇ�͉������Ȃ�.
*/
void reset_image(int no);

/**
* ��ʂ��t�F�[�h�A�E�g����.
*
* @param  red     �t�F�[�h�A�E�g�t�B���^�[�̐Ԑ���(0.0�`1.0).
* @param  green   �t�F�[�h�A�E�g�t�B���^�[�̗ΐ���(0.0�`1.0).
* @param  blue    �t�F�[�h�A�E�g�t�B���^�[�̐���(0.0�`1.0).
* @param  seconds ���쎞��(�b).
*/
void fade_out(float red, float green, float blue, float seconds);

/**
* ��ʂ��t�F�[�h�C������.
*
* @param  seconds ���쎞��(�b).
*/
void fade_in(float seconds);

/**
* ��莞�ԑ҂�.
*
* @param  seconds �҂�����(�b).
*/
void wait(float seconds);

/**
* �����L�[�����͂����܂ő҂�.
*/
void wait_any_key();

/**
* �I������\�����āA�I�����ꂽ���ʂ𓾂�.
*
* @param  x     �\���J�n�ʒu�̍��[���W.
* @param  y     �\���J�n�ʒu�̏�[���W.
* @param  count �I�����̐�.
* @param  a     �ЂƂ߂̑I����.
* @param  b     �ӂ��ڂ̑I����.
* @param  ...   �݂��ȏ�̑I����������Ȃ�A�J���}�ŋ�؂��Ēǉ����Ă���.
*
* @return �I�����ꂽ���ڂ̔ԍ�.
*         �ЂƂ߂̑I������0�ԁA�ӂ��߂̑I������1�ԂƂ��A�ȍ~��2,3,4�̂悤��1���ԍ��������Ă���.
*
* �\���J�n�ʒu�̓E�B���h�E�̒��S�����_�Ƃ��A���E��X��(�E���v���X����)�A�㉺��Y��(�オ�v���X����)�̍��W�n�Ŏw�肷��.
* �E�B���h�E�̑傫���͉�800�h�b�g�A�c600�h�b�g�ł���.
* �I�����͂ЂƂ����s����ĉ��ɕ��Ԃ̂ŁA�I�����𑝂₷�ۂ̓E�B���h�E���͂ݏo���Ȃ��悤�ɒ��ӂ��邱��.
*/
int select(float x, float y, int count, const char* a, const char* b, ...);

/**
* �����𓾂�.
*
* @param  min ��肤��͈͂̍ŏ��l.
* @param  max ��肤��͈͂̍ő�l.
*
* @return min�ȏ�max�ȉ��̃����_���Ȓl.
*/
int random(int min, int max);

/**
* �A�v���P�[�V�������I������.
*/
void quit();

/**
* �������Đ�����.
*
* @param filename �Đ����鉹���t�@�C��.
*/
void play_sound(const char* filename);

/**
* �w�i���y���Đ�����.
*
* @param filename �Đ����鉹���t�@�C��.
*/
void play_bgm(const char* filename);

/**
* �w�i���y���~����.
*/
void stop_bgm();

/**
* �����̉��ʂ�ݒ肷��.
*
* @param volume ����
*               0.0 ����
*               1.0 �����f�[�^���̂܂܂̉���.
*               2.0 �����f�[�^��2�{�̉���.
*/
void set_sound_volume(float volume);

/**
* �w�i���y�̉��ʂ�ݒ肷��.
*
* @param volume ����
*               0.0 ����
*               1.0 �����f�[�^���̂܂܂̉���.
*               2.0 �����f�[�^��2�{�̉���.
*/
void set_bgm_volume(float volume);

#endif // COMMAND_H_INCLUDED
