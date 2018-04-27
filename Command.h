#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

/**
* 文字を表示する.
*
* @param  x      表示開始位置の左端座標.
* @param  y      表示開始位置の上端座標.
* @param  format 文字の書式指定(printf関数のものと同じ).
* @param  ...    追加の引数(printf関数のものと同じ).
*/
void set_text(int x, int y, const char* format, ...);

/**
* すべての文字を消す.
*/
void reset_all_text();

/**
* 画像を配置する.
*
* @param  no       画像の管理番号.
* @param  x        表示位置(X座標).
* @param  y        表示位置(Y座標).
* @param  filename 画像ファイルの名前.
*
* noで指定した管理番号に既に別の画像を配置していた場合、
* まずその画像を消去してから、改めてfilenameで指定した画像を配置する.
*/
void set_image(int no, float x, float y, const char* filename);

/**
* すべての画像を消す.
*/
void reset_all_image();

/**
* 画像を消す.
*
* @param  no 画像の管理番号.
*
* 対象の画像がすでに消されていた場合は何もしない.
*/
void reset_image(int no);

/**
* 一定時間待つ.
*
* @param  seconds 待ち時間(秒).
*/
void wait(float seconds);

/**
* 選択肢を表示して、選択された結果を得る.
*
* @param  x     表示開始位置の左端座標.
* @param  y     表示開始位置の上端座標.
* @param  count 選択肢の数.
* @param  a     ひとつめの選択肢.
* @param  b     ふたつ目の選択肢.
* @param  ...   みっつ以上の選択肢があるなら、カンマで区切って追加していく.
*
* @return 選択された項目の番号.
*         ひとつめの選択肢を0番とする.
*/
int select(int x, int y, int count, const char* a, const char* b, ...);

/**
* 乱数を得る.
*
* @param  min 取りうる範囲の最小値.
* @param  max 取りうる範囲の最大値.
*
* @return min以上max以下のランダムな値.
*/
int random(int min, int max);

/**
* アプリケーションを終了する.
*/
void quit();


#endif // COMMAND_H_INCLUDED
