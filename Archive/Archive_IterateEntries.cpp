#include "Archive.h"

void* Archive_IterateEntries(ArchiveIterator* iter, uint32_t* outKey)
{
    if (iter->currentNode == nullptr) {
        if (iter->bucketIndex >= 0)
            return 0; // already exhausted
        // first call: fall through to bucket scan
    } else {
        iter->currentNode = iter->currentNode->next; // advance chain
    }

    if (iter->currentNode == nullptr) {
        // find next non-empty bucket
        int bucketCount = 1 << (iter->log2BucketCount & 0x1F);
        do {
            if (++iter->bucketIndex >= bucketCount)
                return 0;
        } while (iter->buckets[iter->bucketIndex].head    == nullptr &&
                 iter->buckets[iter->bucketIndex].headAlt == nullptr);

        ArchiveBucket& b = iter->buckets[iter->bucketIndex];
        iter->currentNode = (b.head != nullptr) ? b.head : b.headAlt;
    }

    if (iter->currentNode == nullptr)
        return 0;

    if (outKey != nullptr)
        *outKey = iter->currentNode->key;

    return iter->currentNode->archive;
}