#include <iostream>
#include <cmath>

#include <stdlib.h>
#include <memory.h>

#include "common/newbie_log.h"

#include "BinaryTreeGraphic.h"

using namespace std;
using namespace NewbieC;

BinaryTreeGraphic::BinaryTreeGraphic(LinkFillChar *pLinkFillCharType)
    : m_linkFillCharType(*pLinkFillCharType)
    , m_pLeafNodeIndexRecord(NULL)
{
    return;
}

INT32 BinaryTreeGraphic::DrawBinaryTreeGraphic(INT32 *pBTArray, INT32 BTArrayNodeCount)
{
    FLOAT64 nodeCount       = FLOAT64(BTArrayNodeCount);
    FLOAT64 nodeLevelCountF = log2(nodeCount+1);
    INT32   nodeLevelCount  = INT32(nodeLevelCountF+0.5);

    BTGraphicNode *tmpBTGraphicNode = reinterpret_cast<BTGraphicNode *>(malloc(BTArrayNodeCount*sizeof(BTGraphicNode)));
    m_pLeafNodeIndexRecord          = reinterpret_cast<INT32 *>(malloc(nodeLevelCount*sizeof(INT32)));
    if ((NULL == tmpBTGraphicNode) || (NULL == m_pLeafNodeIndexRecord))
    {
        if (NULL != tmpBTGraphicNode)
        {
            free(tmpBTGraphicNode);
        }

        if (NULL != m_pLeafNodeIndexRecord)
        {
            free(m_pLeafNodeIndexRecord);
        }

        NEWBIE_ERROR(GRAPHIC_LOG_GRP, "alloc failed, check the system memory");
        return ErrorNoMemory;
    }
    memset(tmpBTGraphicNode, 0, BTArrayNodeCount*sizeof(BTGraphicNode));
    memset(m_pLeafNodeIndexRecord, 0, nodeLevelCount*sizeof(INT32));

    NEWBIE_INFO(GRAPHIC_LOG_GRP, "BTArrayNodeCount %d, nodeCount %f, nodeLevelCount %d, nodeLevelCountF %f",
        BTArrayNodeCount, nodeCount, nodeLevelCount, nodeLevelCountF);

    INT32 nodeLevelStartIdx = 2;
    INT32 nodeLevelEndIdx   = 0;
    INT32 nodeLevelIdx      = 0;
    INT32 nodeTreeLevelIdx  = 0;

    // Forward traverse to fill main member of binary graphic tree.
    for (INT32 nodeIdx = 0; nodeIdx < BTArrayNodeCount; nodeIdx++)
    {
        INT32 leftSubNodeIdx    = (nodeIdx+1) * 2 - 1;
        INT32 rightSubNodeIdx   = leftSubNodeIdx + 1;

        if (leftSubNodeIdx > BTArrayNodeCount - 1)
        {
            tmpBTGraphicNode[nodeIdx].leftBTSubNode     = NULL;
        }
        else
        {
            tmpBTGraphicNode[nodeIdx].leftBTSubNode     = &tmpBTGraphicNode[leftSubNodeIdx];
        }

        if (rightSubNodeIdx > BTArrayNodeCount - 1)
        {
            tmpBTGraphicNode[nodeIdx].rightBTSubNode     = NULL;
        }
        else
        {
            tmpBTGraphicNode[nodeIdx].rightBTSubNode     = &tmpBTGraphicNode[rightSubNodeIdx];
        }

        tmpBTGraphicNode[nodeIdx].nodeValue         = pBTArray[nodeIdx];

        if ((nodeIdx + 1) == nodeLevelStartIdx)
        {
            nodeTreeLevelIdx++;
            nodeLevelIdx = 0;
            nodeLevelStartIdx = nodeLevelStartIdx * 2;
            NEWBIE_INFO(GRAPHIC_LOG_GRP, "Got Tree level number %d", nodeTreeLevelIdx);
        }
        tmpBTGraphicNode[nodeIdx].treeLevel         = nodeTreeLevelIdx;
        tmpBTGraphicNode[nodeIdx].leafNodeIndex     = nodeLevelIdx++;
        tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot  = std::to_string(pBTArray[nodeIdx]).length();
    }

    TraverseBT(tmpBTGraphicNode, TraverseType::PreOrderTraversal);
    TraverseBT(tmpBTGraphicNode, TraverseType::InOrderTraversal);
    TraverseBT(tmpBTGraphicNode, TraverseType::PostOrderTraversal);

    // Backward traverse to fill the node print coordinates.
    nodeLevelStartIdx = nodeLevelStartIdx - 1;
    nodeLevelEndIdx = nodeLevelStartIdx;
    while (nodeLevelStartIdx > 0)
    {
        nodeLevelStartIdx = nodeLevelStartIdx / 2;
        NEWBIE_INFO(GRAPHIC_LOG_GRP, "nodeLevelStartIdx %d, nodeLevelEndIdx %d",
            nodeLevelStartIdx, nodeLevelEndIdx);
 
        for (INT32 i = nodeLevelStartIdx; i < nodeLevelEndIdx && i < BTArrayNodeCount; i++)
        {
            if (HasFullLeafNode(&tmpBTGraphicNode[i]))
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor +
                    (tmpBTGraphicNode[i].rightBTSubNode->coorOfNodeRoot.xCoor -
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor) / 2;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor += 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;
            }
            else if (NULL != tmpBTGraphicNode[i].leftBTSubNode)
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor +
                    tmpBTGraphicNode[i].leftBTSubNode->lengthOfNodeRoot + 1;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor -= 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;
            }
            else if (NULL != tmpBTGraphicNode[i].rightBTSubNode)
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].rightBTSubNode->coorOfNodeRoot.xCoor - 1 - tmpBTGraphicNode[i].lengthOfNodeRoot;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor -= 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;
            }
            else
            {
                if (i > nodeLevelStartIdx)
                {
                    tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                        tmpBTGraphicNode[i-1].coorOfNodeRoot.xCoor + tmpBTGraphicNode[i-1].lengthOfNodeRoot + 2;
                }
            }

            tmpBTGraphicNode[i].coorOfNodeRoot.yCoor = nodeTreeLevelIdx;
        }
        nodeTreeLevelIdx--;
        nodeLevelEndIdx = nodeLevelStartIdx;
    }

    // Forward traverse to print the binary graphic tree.
    INT32 stringCacheIdx = 0;
    nodeLevelStartIdx = 1;

    // Forward traverse the binary tree and scan it level by level
    nodeLevelStartIdx = 0;
    nodeLevelEndIdx = nodeLevelStartIdx * 2 + 1;

    // Fill the default output char with "BlankFillType" which indicated by user.
    memset(m_printCache, m_linkFillCharType.BlankFillType, sizeof(m_printCache));
    memset(m_printCacheEnter, m_linkFillCharType.BlankFillType, sizeof(m_printCacheEnter));
    while (nodeLevelStartIdx < BTArrayNodeCount)
    {
        UINT32 enterCharCoor = 0;
        for (INT32 nodeIdx = nodeLevelStartIdx; nodeIdx < nodeLevelEndIdx && nodeIdx < BTArrayNodeCount; nodeIdx++)
        {
            // Non-last leaf node level, print link line, value, enter line.
            if (HasLeafNode(&tmpBTGraphicNode[nodeIdx]))
            {
                if (NULL != tmpBTGraphicNode[nodeIdx].leftBTSubNode)
                {
                    INT32 linkLineStart = tmpBTGraphicNode[nodeIdx].leftBTSubNode->coorOfNodeRoot.xCoor +
                        (tmpBTGraphicNode[nodeIdx].leftBTSubNode->lengthOfNodeRoot / 2) + 1;
                    stringCacheIdx = linkLineStart;
                    for (INT32 i = linkLineStart; i < tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor; i++)
                    {
                        m_printCache[stringCacheIdx++] = m_linkFillCharType.LinkFillType;
                    }

                    enterCharCoor = tmpBTGraphicNode[nodeIdx].leftBTSubNode->coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].leftBTSubNode->lengthOfNodeRoot / 2;
                    m_printCacheEnter[enterCharCoor] = m_linkFillCharType.LeftFillType;

                }
                memcpy(&m_printCache[stringCacheIdx],
                    std::to_string(tmpBTGraphicNode[nodeIdx].nodeValue).c_str(), tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot);
                stringCacheIdx += tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;

                if (NULL != tmpBTGraphicNode[nodeIdx].rightBTSubNode)
                {
                    INT32 linkLineStart = tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;
                    
                    // We decrease 1, because the "\" line is looks like below if we do not decrease 1
                    // First cloum no decrease, second cloum has decrease
                    // "\   \ "
                    // "6   6 "
                    INT32 linkLineEnd   = tmpBTGraphicNode[nodeIdx].rightBTSubNode->coorOfNodeRoot.xCoor +
                        (tmpBTGraphicNode[nodeIdx].rightBTSubNode->lengthOfNodeRoot / 2) - 1;

                    stringCacheIdx = linkLineStart;
                    for (INT32 i = linkLineStart; i < linkLineEnd; i++)
                    {
                        m_printCache[stringCacheIdx++] = m_linkFillCharType.LinkFillType;
                    }

                    enterCharCoor = tmpBTGraphicNode[nodeIdx].rightBTSubNode->coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].rightBTSubNode->lengthOfNodeRoot / 2 - 1;
                    m_printCacheEnter[enterCharCoor] = m_linkFillCharType.rightFillType;
                }
            }
            else
            {
                stringCacheIdx = tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor;
                // last leaf node level, just print node value.
                memcpy(&m_printCache[stringCacheIdx],
                    std::to_string(tmpBTGraphicNode[nodeIdx].nodeValue).c_str(), tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot);
                stringCacheIdx += tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;
            }
        }

        m_printCache[stringCacheIdx] = 0; // End char.
        printf("\033[0;32;32m%s\033[m\n", m_printCache);
        // Fill the default output char with "BlankFillType" which indicated by user.
        memset(m_printCache, m_linkFillCharType.BlankFillType, sizeof(m_printCache));

        m_printCacheEnter[enterCharCoor+1] = 0; // End char.
        printf("\033[0;32;32m%s\033[m\n", m_printCacheEnter);
        // Fill the default output char with "BlankFillType" which indicated by user.
        memset(m_printCacheEnter, m_linkFillCharType.BlankFillType, sizeof(m_printCacheEnter));

        stringCacheIdx = 0;
        nodeLevelStartIdx = nodeLevelStartIdx * 2 + 1;
        nodeLevelEndIdx   = nodeLevelStartIdx * 2 + 1;
    }

    if (NULL != tmpBTGraphicNode)
    {
        free(tmpBTGraphicNode);
    }

    if (NULL != m_pLeafNodeIndexRecord)
    {
        free(m_pLeafNodeIndexRecord);
    }

    return RetSuccess;
}

INT32 BinaryTreeGraphic::DrawBinaryTreeGraphic(INT32 **ppBTArray, INT32 BTArrayNodeCount)
{
    FLOAT64 nodeCount       = FLOAT64(BTArrayNodeCount);
    FLOAT64 nodeLevelCountF = log2(nodeCount+1);
    INT32   nodeLevelCount  = INT32(nodeLevelCountF+0.5);

    BTGraphicNode *tmpBTGraphicNode = reinterpret_cast<BTGraphicNode *>(malloc(BTArrayNodeCount*sizeof(BTGraphicNode)));
    m_pLeafNodeIndexRecord          = reinterpret_cast<INT32 *>(malloc(nodeLevelCount*sizeof(INT32)));
    if ((NULL == tmpBTGraphicNode) || (NULL == m_pLeafNodeIndexRecord))
    {
        if (NULL != tmpBTGraphicNode)
        {
            free(tmpBTGraphicNode);
        }

        if (NULL != m_pLeafNodeIndexRecord)
        {
            free(m_pLeafNodeIndexRecord);
        }

        NEWBIE_ERROR(GRAPHIC_LOG_GRP, "alloc failed, check the system memory");
        return ErrorNoMemory;
    }
    memset(tmpBTGraphicNode, 0, BTArrayNodeCount*sizeof(BTGraphicNode));
    memset(m_pLeafNodeIndexRecord, 0, nodeLevelCount*sizeof(INT32));

    NEWBIE_INFO(GRAPHIC_LOG_GRP, "BTArrayNodeCount %d, nodeCount %f, nodeLevelCount %d, nodeLevelCountF %f",
        BTArrayNodeCount, nodeCount, nodeLevelCount, nodeLevelCountF);

    INT32 nodeLevelStartIdx = 2;
    INT32 nodeLevelEndIdx   = 0;
    INT32 nodeLevelIdx      = 0;
    INT32 nodeTreeLevelIdx  = 0;

    // Forward traverse to fill main member of binary graphic tree and build binary graphic tree.
    for (INT32 nodeIdx = 0; nodeIdx < BTArrayNodeCount; nodeIdx++)
    {
        INT32 leftSubNodeIdx    = nodeIdx * 2 + 1;
        INT32 rightSubNodeIdx   = leftSubNodeIdx + 1;

        if (leftSubNodeIdx > BTArrayNodeCount - 1)
        {
            tmpBTGraphicNode[nodeIdx].leftBTSubNode     = NULL;
        }
        else
        {
            tmpBTGraphicNode[nodeIdx].leftBTSubNode     = &tmpBTGraphicNode[leftSubNodeIdx];
        }

        if (rightSubNodeIdx > BTArrayNodeCount - 1)
        {
            tmpBTGraphicNode[nodeIdx].rightBTSubNode     = NULL;
        }
        else
        {
            tmpBTGraphicNode[nodeIdx].rightBTSubNode     = &tmpBTGraphicNode[rightSubNodeIdx];
        }

        if ((nodeIdx + 1) == nodeLevelStartIdx)
        {
            nodeTreeLevelIdx++;
            nodeLevelIdx = 0;
            nodeLevelStartIdx = nodeLevelStartIdx * 2;
            NEWBIE_INFO(GRAPHIC_LOG_GRP, "Got Tree level number %d", nodeTreeLevelIdx);
        }
        tmpBTGraphicNode[nodeIdx].treeLevel         = nodeTreeLevelIdx;
        tmpBTGraphicNode[nodeIdx].leafNodeIndex     = nodeLevelIdx++;

        if (NULL == ppBTArray[nodeIdx])
        {
            tmpBTGraphicNode[nodeIdx].nodeValue         = 0;
            tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot  = 0;
            tmpBTGraphicNode[nodeIdx].bIsValid          = 0;
        }
        else
        {
            tmpBTGraphicNode[nodeIdx].nodeValue         = *ppBTArray[nodeIdx];
            tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot  = std::to_string(*ppBTArray[nodeIdx]).length();
            tmpBTGraphicNode[nodeIdx].bIsValid          = 1;
        }
    }

    // Debug, traverse the node and print
    TraverseBT(tmpBTGraphicNode, TraverseType::PreOrderTraversal);
    TraverseBT(tmpBTGraphicNode, TraverseType::InOrderTraversal);
    TraverseBT(tmpBTGraphicNode, TraverseType::PostOrderTraversal);

    // Backward traverse to fill the node print coordinates.
    // for loop is level by level backward
    nodeLevelStartIdx = nodeLevelStartIdx - 1;
    nodeLevelEndIdx   = nodeLevelStartIdx;
    while (nodeLevelStartIdx > 0)
    {
        nodeLevelStartIdx = nodeLevelStartIdx / 2;
        NEWBIE_INFO(GRAPHIC_LOG_GRP, "nodeLevelStartIdx %d, nodeLevelEndIdx %d",
            nodeLevelStartIdx, nodeLevelEndIdx);
 
        for (INT32 i = nodeLevelStartIdx; i < nodeLevelEndIdx && i < BTArrayNodeCount; i++)
        {
            if (HasFullLeafNode(&tmpBTGraphicNode[i]))
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor +
                    (tmpBTGraphicNode[i].rightBTSubNode->coorOfNodeRoot.xCoor -
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor) / 2;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor -= 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;                
            }
            else if (NULL != tmpBTGraphicNode[i].leftBTSubNode)
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].leftBTSubNode->coorOfNodeRoot.xCoor +
                    tmpBTGraphicNode[i].leftBTSubNode->lengthOfNodeRoot + 1;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor -= 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;
            }
            else if (NULL != tmpBTGraphicNode[i].rightBTSubNode)
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                    tmpBTGraphicNode[i].rightBTSubNode->coorOfNodeRoot.xCoor - 1 - tmpBTGraphicNode[i].lengthOfNodeRoot;
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor -= 
                    tmpBTGraphicNode[i].lengthOfNodeRoot / 2;
            }
            else
            {
                // Completely leaf node, just based on last leaf node's coordinate.
                if (i > nodeLevelStartIdx)
                {
                    tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 
                        tmpBTGraphicNode[i-1].coorOfNodeRoot.xCoor + tmpBTGraphicNode[i-1].lengthOfNodeRoot + 2;
                }
            }
            if (tmpBTGraphicNode[i].coorOfNodeRoot.xCoor < 0)
            {
                tmpBTGraphicNode[i].coorOfNodeRoot.xCoor = 0;
            }
            tmpBTGraphicNode[i].coorOfNodeRoot.yCoor = nodeTreeLevelIdx;
        }
        nodeTreeLevelIdx--;
        nodeLevelEndIdx = nodeLevelStartIdx;
    }

    // Forward traverse to print the binary graphic tree.
    INT32 stringCacheIdx = 0;

    // Forward traverse the binary tree and scan it level by level
    nodeLevelStartIdx = 0;
    nodeLevelEndIdx   = nodeLevelStartIdx * 2 + 1;

    // Fill the default output char with "BlankFillType" which indicated by user.
    memset(m_printCache, m_linkFillCharType.BlankFillType, sizeof(m_printCache));
    memset(m_printCacheEnter, m_linkFillCharType.BlankFillType, sizeof(m_printCacheEnter));
    while (nodeLevelStartIdx < BTArrayNodeCount)
    {
        UINT32 enterCharCoor = 0;
        for (INT32 nodeIdx = nodeLevelStartIdx; nodeIdx < nodeLevelEndIdx && nodeIdx < BTArrayNodeCount; nodeIdx++)
        {
            // Non-last leaf node level, print link line, value, enter line.
            if (HasLeafNode(&tmpBTGraphicNode[nodeIdx]))
            {
                if (NULL != tmpBTGraphicNode[nodeIdx].leftBTSubNode)
                {
                    INT32 linkLineStart = tmpBTGraphicNode[nodeIdx].leftBTSubNode->coorOfNodeRoot.xCoor +
                        (tmpBTGraphicNode[nodeIdx].leftBTSubNode->lengthOfNodeRoot / 2) + 1;
                    stringCacheIdx = linkLineStart;
                    for (INT32 i = linkLineStart; i < tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor; i++)
                    {
                        if (tmpBTGraphicNode[nodeIdx].leftBTSubNode->bIsValid)
                        {
                            m_printCache[stringCacheIdx] = m_linkFillCharType.LinkFillType;
                        }
                        stringCacheIdx++;
                    }

                    enterCharCoor = tmpBTGraphicNode[nodeIdx].leftBTSubNode->coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].leftBTSubNode->lengthOfNodeRoot / 2;
                    
                    if (tmpBTGraphicNode[nodeIdx].leftBTSubNode->bIsValid)
                    {
                        m_printCacheEnter[enterCharCoor] = m_linkFillCharType.LeftFillType;
                    }
                }
                if (tmpBTGraphicNode[nodeIdx].bIsNullString)
                {
                    memcpy(&m_printCache[stringCacheIdx],
                        "null", tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot);
                }
                else
                {
                    memcpy(&m_printCache[stringCacheIdx],
                        std::to_string(tmpBTGraphicNode[nodeIdx].nodeValue).c_str(), tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot);
                }
                stringCacheIdx += tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;

                if (NULL != tmpBTGraphicNode[nodeIdx].rightBTSubNode)
                {
                    INT32 linkLineStart = tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;
                    
                    // We decrease 1, because the "\" line is looks like below if we do not decrease 1
                    // First cloum no decrease, second cloum has decrease
                    // "\   \ "
                    // "6   6 "
                    INT32 linkLineEnd   = tmpBTGraphicNode[nodeIdx].rightBTSubNode->coorOfNodeRoot.xCoor +
                        (tmpBTGraphicNode[nodeIdx].rightBTSubNode->lengthOfNodeRoot / 2) - 1;

                    stringCacheIdx = linkLineStart;
                    for (INT32 i = linkLineStart; i < linkLineEnd; i++)
                    {
                        if (tmpBTGraphicNode[nodeIdx].rightBTSubNode->bIsValid)
                        {
                            m_printCache[stringCacheIdx] = m_linkFillCharType.LinkFillType;
                        }
                        stringCacheIdx++;
                    }

                    enterCharCoor = tmpBTGraphicNode[nodeIdx].rightBTSubNode->coorOfNodeRoot.xCoor +
                        tmpBTGraphicNode[nodeIdx].rightBTSubNode->lengthOfNodeRoot / 2 - 1;

                    if (tmpBTGraphicNode[nodeIdx].rightBTSubNode->bIsValid)
                    {
                        m_printCacheEnter[enterCharCoor] = m_linkFillCharType.rightFillType;
                    }
                }
            }
            else
            {
                stringCacheIdx = tmpBTGraphicNode[nodeIdx].coorOfNodeRoot.xCoor;
                // last leaf node level, just print node value.
                memcpy(&m_printCache[stringCacheIdx],
                    std::to_string(tmpBTGraphicNode[nodeIdx].nodeValue).c_str(), tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot);
                stringCacheIdx += tmpBTGraphicNode[nodeIdx].lengthOfNodeRoot;
            }
        }

        m_printCache[stringCacheIdx] = 0; // End char.
        printf("\033[0;32;32m%s\033[m\n", m_printCache);
        // Fill the default output char with "BlankFillType" which indicated by user.
        memset(m_printCache, m_linkFillCharType.BlankFillType, sizeof(m_printCache));

        m_printCacheEnter[enterCharCoor+1] = 0; // End char.
        printf("\033[0;32;32m%s\033[m\n", m_printCacheEnter);
        // Fill the default output char with "BlankFillType" which indicated by user.
        memset(m_printCacheEnter, m_linkFillCharType.BlankFillType, sizeof(m_printCacheEnter));

        stringCacheIdx = 0;
        nodeLevelStartIdx = nodeLevelStartIdx * 2 + 1;
        nodeLevelEndIdx   = nodeLevelStartIdx * 2 + 1;
    }

    if (NULL != tmpBTGraphicNode)
    {
        free(tmpBTGraphicNode);
    }

    if (NULL != m_pLeafNodeIndexRecord)
    {
        free(m_pLeafNodeIndexRecord);
    }

    return RetSuccess;
}

#define BINARY_TREE_ARRAY_LENGTH    80

INT32 main(INT32 argc, CHAR *argv[])
{
    LinkFillChar linkFillChar = { '_', ' ', '/', '\\' };
    BinaryTreeGraphic binaryTree(&linkFillChar);

    INT32 binaryTreeArray[BINARY_TREE_ARRAY_LENGTH];
    INT32 *pBinaryTreeArray[BINARY_TREE_ARRAY_LENGTH];

    for (INT32 i = 0; i < BINARY_TREE_ARRAY_LENGTH; i++)
    {
        binaryTreeArray[i] = random() % BINARY_TREE_ARRAY_LENGTH;
        // NEWBIE_INFO(GRAPHIC_LOG_GRP, "Index %d, value %d", i, binaryTreeArray[i]);
    }

    for (INT32 i = 0; i < BINARY_TREE_ARRAY_LENGTH; i++)
    {
        if ((i > 0) && (NULL == pBinaryTreeArray[(i-1)/2]))
        {
            pBinaryTreeArray[i] = NULL;
        }
        else if ((0 == binaryTreeArray[i] % 7) ||
             (random() % BINARY_TREE_ARRAY_LENGTH == binaryTreeArray[i]))
        {
            pBinaryTreeArray[i] = NULL;
        }
        else
        {
            pBinaryTreeArray[i] = &binaryTreeArray[i];
        }
    }

    //binaryTree.DrawBinaryTreeGraphic(binaryTreeArray, BINARY_TREE_ARRAY_LENGTH);
    binaryTree.DrawBinaryTreeGraphic(pBinaryTreeArray, BINARY_TREE_ARRAY_LENGTH);

    return RetSuccess;
}
