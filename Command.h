#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

/**
* ������\������.
*
* @param  x      �\���J�n�ʒu�̍��[���W.
* @param  y      �\���J�n�ʒu�̏�[���W.
* @param  format �����̏����w��(printf�֐��̂��̂Ɠ���).
* @param  ...    �ǉ��̈���(printf�֐��̂��̂Ɠ���).
*/
void set_text(int x, int y, const char* format, ...);

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
* @param  seconds ���쎞��(�b).
*/
void move_image(int no, float x, float y, int easing, float seconds);

/**
* �摜���g��E�k������.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  x       �g��E�k�����X�����̑傫��.
* @param  y       �g��E�k�����Y�����̑傫��..
* @param  easing  ��ԕ��@:
*                   0 �����Ŋg��E�k��.
*                   1 �������Ȃ���g��E�k��.
*                   2 �������Ȃ���g��E�k��.
*                   3 �������Ȃ���g��E�k�����n�߂āA�������Ȃ����~����.
* @param  seconds ���쎞��(�b).
*/
void scale_image(int no, float x, float y, int easing, float seconds);

/**
* �摜����]����.
*
* @param  no      �摜�̊Ǘ��ԍ�.
* @param  degree  ��]������p�x(�x���@).
* @param  easing  ��ԕ��@:
*                   0 �����ŉ�].
*                   1 �������Ȃ����].
*                   2 �������Ȃ����].
*                   3 �������Ȃ����]���n�߂āA�������Ȃ����~����.
* @param  seconds ���쎞��(�b).
*/
void rotate_image(int no, float degree, int easing, float seconds);

/**
* �摜�ɃJ���[�t�B���^�[��ݒ肷��.
*
* @param  no    �摜�̊Ǘ��ԍ�.
* @param  red   �t�B���^�[�̐Ԑ���(0.0�`1.0).
* @param  green �t�B���^�[�̗ΐ���(0.0�`1.0).
* @param  blue  �t�B���^�[�̐���(0.0�`1.0).
* @param  alpha �t�B���^�[�̓����x(0.0�`1.0).
*/
void color_filter(int no, float red, float green, float blue, float alpha);

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
* ���ׂẲ摜������.
*/
void reset_all_image();

/**
* �摜������.
*
* @param  no �摜�̊Ǘ��ԍ�.
*
* �Ώۂ̉摜�����łɏ�����Ă����ꍇ�͉������Ȃ�.
*/
void reset_image(int no);

/**
* ��莞�ԑ҂�.
*
* @param  seconds �҂�����(�b).
*/
void wait(float seconds);

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
*         �ЂƂ߂̑I������0�ԂƂ���.
*/
int select(int x, int y, int count, const char* a, const char* b, ...);

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


#endif // COMMAND_H_INCLUDED
