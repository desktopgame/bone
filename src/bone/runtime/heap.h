#ifndef BONE_RUNTIME_HEAP_H
#define BONE_RUNTIME_HEAP_H
#include "../glib.h"
#include "storage.h"

struct bnHeap;
struct bnObject;
struct bnFrame;
struct bnInterpreter;

#define OBJECT_MAXSIZE (48)

/**
 * 新しいbnHeapインスタンスを生成して返します。
 * @return
 */
struct bnHeap* bnNewHeap();

/**
 * ヒープからまだ使用されていないオブジェクトを検索し、
 * そのオブジェクトへの参照を返します。
 * @param self
 * @return
 */
bnReference bnAllocObject(struct bnHeap* self);

/**
 * 参照をオブジェクトの実際のポインタへ変換します。
 * @param self
 * @param ref
 * @return
 */
void* bnGetObject(struct bnHeap* self, bnReference ref);

/**
 * 参照を未使用としてマークします。
 * @parram self
 * @param ref
 */
void bnFreeObject(struct bnHeap* self, bnReference ref);

/**
 * 新たなステージを生成してプッシュします。
 * @param self
 */
void bnPushStage(struct bnHeap* self);

/**
 * 最後に生成されたステージに参照を追加します。
 * ステージに追加された参照はGCから保護されます。
 * 主にネイティブメソッドのための仕組みです。
 * @param self
 * @param ref
 * @return
 */
bnReference bnStaging(struct bnHeap* self, bnReference ref);

/**
 * 最後に生成されたステージを解放します。
 * @param self
 */
void bnPopStage(struct bnHeap* self);

/**
 * ガベージコレクションとコンパクションを実行します。
 * この呼び出しの前に参照からポインタを得ていた場合、
 * コンパクションによってポインタの意味する値が変わってしまっているため、
 * 再びbnGetObjectによってポインタを取得する必要があります。
 * @param bone
 */
void bnGC(struct bnInterpreter* bone);

/**
 * ヒープが内部的に使用しているストレージを返します。
 * この関数を呼び出すのは好ましくありません。
 * bnAllocObject,bnGetObject,bnFreeObjectで十分なはずです。
 * 主にデバッグのために提供されている関数です。
 * @param self
 * @return
 */
bnStorage* bnGetHeapStorage(struct bnHeap* self);

/**
 * オブジェクトを格納するための領域を解放します。
 * @param self
 */
void bnDeleteHeap(struct bnHeap* self);
#endif