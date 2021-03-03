#ifndef _BINARY_TREE_GRAPHIC_H_
#define _BINARY_TREE_GRAPHIC_H_

#include "common/basic_type.h"
#include "common/basic_graphic.h"

namespace NewbieC {

struct LinkFillChar {
    CHAR LinkFillType;  //
    CHAR BlankFillType;
    CHAR LeftFillType;
    CHAR rightFillType;
};

struct BTGraphicNode {
    INT32           nodeValue;          // This is binary tree's node value
    Coordinate2D    coorOfNodeRoot;     // This is a 2D coordinate, use x, y description
    INT32           lengthOfNodeRoot;   // Node value should be converted into string, this is regarded as string length
    INT32           treeLevel;          // Which level is this node?
    INT32           leafNodeIndex;      // The leaf node index who are in same tree level
    BOOL            bIsValid;           // Is this a valid non-null node?
    BOOL            bIsNullString;      // Is this a NULL string?

    BTGraphicNode   *leftBTSubNode;     // Left sub-node
    BTGraphicNode   *rightBTSubNode;    // Right sub-node
};

class BinaryTreeGraphic {
public:
    BinaryTreeGraphic(LinkFillChar *pLinkFillCharType);
    ~BinaryTreeGraphic()
    {
        return;
    }

    // !!!!!!!!!!!!!!!!!!! DO NOT USE THIS METHOD, USE NEXT ONE !!!!!!!!!!!!!!!!!
    /*
    * This method will get BT from @pBTArray and draw a graphic into display
    * @pBTArray: INT32 array, shoule be full binary tree
    * @BTArrayNodeCount: How many node in this BTArray
    */
    INT32 DrawBinaryTreeGraphic(INT32 *pBTArray, INT32 BTArrayNodeCount);

    /*
    * This method will get BT from @pBTArray and draw a graphic into display
    * @ppBTArray: INT32 pointer array.
    * @BTArrayNodeCount: How many node in this BTArray
    */
    INT32 DrawBinaryTreeGraphic(INT32 **ppBTArray, INT32 BTArrayNodeCount);

    enum class TraverseType {
        PreOrderTraversal,
        InOrderTraversal,
        PostOrderTraversal,
    };

    VOID TraverseBT(BTGraphicNode *pBTGraphicNode, TraverseType traversType)
    {
        if ((NULL == pBTGraphicNode) || (pBTGraphicNode->bIsValid == 0)) return;

        switch (traversType)
        {
            case TraverseType::PreOrderTraversal:
            {
                NEWBIE_INFO(GRAPHIC_LOG_GRP, "Node value: %d", pBTGraphicNode->nodeValue);
                TraverseBT(pBTGraphicNode->leftBTSubNode, traversType);
                TraverseBT(pBTGraphicNode->rightBTSubNode, traversType);
            } break;

            case TraverseType::InOrderTraversal:
            {
                TraverseBT(pBTGraphicNode->leftBTSubNode, traversType);
                NEWBIE_INFO(GRAPHIC_LOG_GRP, "Node value: %d", pBTGraphicNode->nodeValue);
                TraverseBT(pBTGraphicNode->rightBTSubNode, traversType);
            } break;

            case TraverseType::PostOrderTraversal:
            {
                TraverseBT(pBTGraphicNode->leftBTSubNode, traversType);
                TraverseBT(pBTGraphicNode->rightBTSubNode, traversType);
                NEWBIE_INFO(GRAPHIC_LOG_GRP, "Node value: %d", pBTGraphicNode->nodeValue);
            } break;

            default:
            {
                NEWBIE_INFO(GRAPHIC_LOG_GRP, "Node value: %d", pBTGraphicNode->nodeValue);
                TraverseBT(pBTGraphicNode->leftBTSubNode, traversType);
                TraverseBT(pBTGraphicNode->rightBTSubNode, traversType);
            } break;
        }

    }

private:
    BOOL HasLeafNode(BTGraphicNode *pBTGraphicNode)
    {
        return (NULL != pBTGraphicNode->leftBTSubNode) || (NULL != pBTGraphicNode->rightBTSubNode);
    }

    BOOL HasFullLeafNode(BTGraphicNode *pBTGraphicNode)
    {
        return (NULL != pBTGraphicNode->leftBTSubNode) && (NULL != pBTGraphicNode->rightBTSubNode);
    }

    // This will delete the default copy constructor, better use it.
    BinaryTreeGraphic (const BinaryTreeGraphic& BinaryTreeGraphic) = delete;
    BinaryTreeGraphic& operator=(const BinaryTreeGraphic& BinaryTreeGraphic) = delete;

    INT32           *m_pLeafNodeIndexRecord;    // This array elements should be equal to binary tree's level
    LinkFillChar    m_linkFillCharType;         // This is used to indicate the fill chars
    CHAR            m_printCache[1024];         // The line output cache
    CHAR            m_printCacheEnter[1024];    // The enter line output cache
};

}

#endif // End of _BINARY_TREE_GRAPHIC_H_