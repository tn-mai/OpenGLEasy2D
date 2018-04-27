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
