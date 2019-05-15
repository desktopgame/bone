#ifndef BN_RUNTIME_LABEL_H
#define BN_RUNTIME_LABEL_H
/**
 * bnLabel is struct for goto.
 */
typedef struct bnLabel {
        int pos;
        int refCount;
} bnLabel;

/**
 * return new instance of bnLabel.
 * @param pos
 * @return
 */
bnLabel* bnNewLabel(int pos);

/**
 * free a bnLabel.
 * @param self
 */
void bnDeleteLabel(bnLabel* self);
#endif