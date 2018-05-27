#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

/**
* 文字を表示する.
*
* @param  x      表示開始位置の左端座標.
* @param  y      表示開始位置の下端座標.
* @param  format 文字の書式指定(printf関数のものと同じ).
* @param  ...    追加の引数(printf関数のものと同じ).
*
* 表示開始位置はウィンドウの中心を原点とし、左右がX軸(右がプラス方向)、上下がY軸(上がプラス方向)の座標系で指定する.
* ウィンドウの大きさは横800ドット、縦600ドットである.
*/
void set_text(float x, float y, const char* format, ...);

/**
* すべての文字を消す.
*/
void reset_all_text();

/**
* 指定した範囲内の文字を消す.
*
* @param  x      消去範囲の左端座標.
* @param  y      消去範囲の下端座標.
* @param  width  消去範囲の横幅.
* @param  height 消去範囲の縦幅.
*
* 範囲内かどうかの判定には、set_text関数に設定した座標が使われる.
* また、文字列の一部が範囲外にあったとしても、set_textで設定した座標が範囲内にあるなら全て消去される.
*
* 例えばset_textを使って、左端座標に右端まで届くような長い文字列を表示したとする.
* 消去範囲を中央から右端までとした場合、この文字列は1文字も消えない.
* 消去範囲を左端から中央までとした場合、この文字列はすべて消える.
*/
void reset_text_area(float x, float y, float width, float height);

/**
* 画像を配置する.
*
* @param  no       画像の管理番号.
* @param  x        表示位置のX座標.
* @param  y        表示位置のY座標.
* @param  filename 画像ファイルの名前.
*
* このプログラムでは、配置した画像ごとに管理番号を付ける.
* 画像の移動や消去は管理番号を通して行う.
*
* 表示位置はウィンドウの中心を原点とし、左右がX軸(右がプラス方向)、上下がY軸(上がプラス方向)の座標系で指定する.
* ウィンドウの大きさは横800ドット、縦600ドットである.
* ここで指定する座標は画像の中心を指す.
*
* noで指定した管理番号に既に別の画像を配置していた場合、
* まずその画像を消去してから、改めてfilenameで指定した画像を配置する.
*/
void set_image(int no, float x, float y, const char* filename);

/**
* 画像を移動する.
*
* @param  no      画像の管理番号.
* @param  x       移動先のX座標.
* @param  y       移動先のY座標.
* @param  easing  補間方法:
*                   0 等速で移動.
*                   1 加速しながら移動.
*                   2 減速しながら移動.
*                   3 加速しながら移動を始めて、減速しながら停止する.
*                   4 1に似ているが、少し行き過ぎてから戻ってきて停止する.
*                   5 1に似ているが、移動先座標で何度か弾んでから停止する.
* @param  seconds 動作時間(秒).
*
* 位置はウィンドウの中心を原点とし、左右がX軸(右がプラス方向)、上下がY軸(上がプラス方向)の座標系で指定する.
* ウィンドウの大きさは横800ドット、縦600ドットである.
* ここで指定する座標は画像の中心を指す.
*/
void move_image(int no, float x, float y, int easing, float seconds);

/**
* 画像を拡大・縮小する.
*
* @param  no      画像の管理番号.
* @param  x       拡大・縮小後のX方向の大きさ. マイナス値を設定すると左右が反転する.
* @param  y       拡大・縮小後のY方向の大きさ. マイナス値を設定すると上下が反転する.
* @param  easing  補間方法:
*                   0 等速で拡大・縮小.
*                   1 加速しながら拡大・縮小.
*                   2 減速しながら拡大・縮小.
*                   3 加速しながら拡大・縮小を始めて、減速しながら停止する.
*                   4 1に似ているが、少し拡大・縮小し過ぎてから戻ってきて停止する.
*                   5 1に似ているが、何度か弾むように拡大・縮小しなから停止する.
* @param  seconds 動作時間(秒).
*/
void scale_image(int no, float x, float y, int easing, float seconds);

/**
* 画像を回転する.
*
* @param  no      画像の管理番号.
* @param  degree  回転させる角度(度数法). マイナス値を設定すると回転方向が逆になる.
* @param  easing  補間方法:
*                   0 等速で回転.
*                   1 加速しながら回転.
*                   2 減速しながら回転.
*                   3 加速しながら回転を始めて、減速しながら停止する.
*                   4 1に似ているが、少し回転し過ぎてから戻ってきて停止する.
*                   5 1に似ているが、何度か弾みなから停止する.
* @param  seconds 動作時間(秒).
*/
void rotate_image(int no, float degree, int easing, float seconds);

/**
* 画像を傾ける.
*
* @param  no      画像の管理番号.
* @param  scale   傾ける比率. マイナス値を設定すると傾きが逆になる.
* @param  easing  補間方法:
*                   0 等速で傾く.
*                   1 加速しながら傾く.
*                   2 減速しながら傾く.
*                   3 加速しながら傾き始めて、減速しながら停止する.
*                   4 1に似ているが、少し傾き過ぎてから戻ってきて停止する.
*                   5 1に似ているが、何度か弾みなから停止する.
* @param  seconds 動作時間(秒).
*/
void shear_image(int no, float scale, int easing, float seconds);

/**
* 画像と色を合成する.
*
* @param  no      画像の管理番号.
* @param  red     合成する色の赤成分(0.0～1.0).
* @param  green   合成する色の緑成分(0.0～1.0).
* @param  blue    合成する色の青成分(0.0～1.0).
* @param  alpha   合成する色の透明度(0.0～1.0).
* @param  mod     合成方法:
*                   0 乗算
*                   1 加算
*                   2 減算
* @param  easing  補間方法:
*                   0 等速で色を合成.
*                   1 加速しながら色を合成.
*                   2 減速しながら色を合成.
*                   3 加速しながら合成を始めて、減速しながら停止する.
*                   4 1に似ているが、少し色を出し過ぎてから戻ってきて停止する.
*                   5 1に似ているが、何度か弾むように合成しなから停止する.
* @param  seconds 動作時間(秒).
*/
void color_blend_image(int no, float red, float green, float blue, float alpha, int mode, int easing, float seconds);

/**
* すべての画像を消す.
*/
void reset_all_image();

/**
* 管理番号で指定された画像を消す.
*
* @param  no 画像の管理番号.
*
* 対象の画像がすでに消されていた場合は何もしない.
*/
void reset_image(int no);

/**
* 画面をフェードアウトする.
*
* @param  red     フェードアウトフィルターの赤成分(0.0～1.0).
* @param  green   フェードアウトフィルターの緑成分(0.0～1.0).
* @param  blue    フェードアウトフィルターの青成分(0.0～1.0).
* @param  seconds 動作時間(秒).
*/
void fade_out(float red, float green, float blue, float seconds);

/**
* 画面をフェードインする.
*
* @param  seconds 動作時間(秒).
*/
void fade_in(float seconds);

/**
* 一定時間待つ.
*
* @param  seconds 待ち時間(秒).
*/
void wait(float seconds);

/**
* 何かキーが入力されるまで待つ.
*/
void wait_any_key();

/**
* ゲーム操作用のキーが入力されるまで待つ.
*
* @param trigger  false キーが押されていれば入力とみなす.
*                 true  キーが押された瞬間だけを入力とみなす.
*
* @retval  0  上キー
* @retval  1  右キー
* @retval  2  下キー
* @retval  3  左キー
* @retval  4  STARTボタン
* @retval  5  Aボタン
* @retval  6  Bボタン
* @retval  7  Xボタン
* @retval  8  Yボタン
* @retval  9  Lボタン
* @retval 10  Rボタン
*/
int wait_game_key(bool trigger);

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
*         ひとつめの選択肢を0番、ふたつめの選択肢を1番とし、以降は2,3,4のように1ずつ番号が増えていく.
*
* 表示開始位置はウィンドウの中心を原点とし、左右がX軸(右がプラス方向)、上下がY軸(上がプラス方向)の座標系で指定する.
* ウィンドウの大きさは横800ドット、縦600ドットである.
* 選択肢はひとつずつ改行されて下に並ぶので、選択肢を増やす際はウィンドウをはみ出さないように注意すること.
*/
int select(float x, float y, int count, const char* a, const char* b, ...);

/**
* 指定した範囲の数値から、選択された値を得る.
*
* @param  x     表示開始位置の左端座標.
* @param  y     表示開始位置の上端座標.
* @param  min   選択範囲の最小値.
* @param  max   選択範囲の最大値.
*
* @return min以上max未満の選択された値.
*
* 表示開始位置はウィンドウの中心を原点とし、左右がX軸(右がプラス方向)、上下がY軸(上がプラス方向)の座標系で指定する.
* ウィンドウの大きさは横800ドット、縦600ドットである.
* 値は上下キーで選択し、決定キー(AボタンorEnterキー)で確定する.
* LまたはRキー(キーボードの場合左Ctrlキーまたは左Shiftキー)を押しながら上下キーを押すと10刻みで選択できる.
*/
int select_number(float x, float y, int min, int max);

/**
* 文字選択パネルを表示し、選択された文字列を得る.
*
* @param  x      表示開始位置の左端座標.
* @param  y      表示開始位置の上端座標.
* @param  max    選択できる文字の数.
* @param  buffer 選択肢た文字を格納するバッファ.
*                格納される文字はSJISの全角文字.
*                全角文字の1文字は2バイト、また最後の文字のあとに終端記号(1バイト)が格納される.
*                このため、バッファの大きさは少なくとも(max * 2 + 1)バイト必要.
*
*/
void select_string(float x, float y, int max, char* buffer);

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

/**
* 音声を再生する.
*
* @param filename 再生する音声ファイル.
*/
void play_sound(const char* filename);

/**
* 背景音楽を再生する.
*
* @param filename 再生する音声ファイル.
*/
void play_bgm(const char* filename);

/**
* 背景音楽を停止する.
*/
void stop_bgm();

/**
* 音声の音量を設定する.
*
* @param volume 音量
*               0.0 無音
*               1.0 音声データそのままの音量.
*               2.0 音声データの2倍の音量.
*/
void set_sound_volume(float volume);

/**
* 背景音楽の音量を設定する.
*
* @param volume 音量
*               0.0 無音
*               1.0 音声データそのままの音量.
*               2.0 音声データの2倍の音量.
*/
void set_bgm_volume(float volume);

#endif // COMMAND_H_INCLUDED
