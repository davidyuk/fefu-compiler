#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "enums.h"

NodeN * createNode0(NodeAction action) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 0;
    node->nodes = 0;
    return node;
}

NodeN * createNode1(NodeAction action, NodeSyntax * node1) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 1;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    return node;
}

NodeN * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 2;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    return node;
}

NodeN * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 3;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    node->nodes[2] = node3;
    return node;
}

NodeN * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 4;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    node->nodes[2] = node3;
    node->nodes[3] = node4;
    return node;
}

NodeN * createNodeN(NodeAction action) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->nodes = 0;
    node->length = 0;
    return node;
}

NodeN * appendNodeN(NodeN * nodeBase, NodeSyntax * nodeNew) {
    if (!nodeNew) return nodeBase;
    assert(nodeBase->type == NODE_N);
    nodeBase->length++;
    nodeBase->nodes = realloc(nodeBase->nodes, sizeof(NodeSyntax *) * nodeBase->length);
    nodeBase->nodes[nodeBase->length-1] = nodeNew;
    return nodeBase;
}

NodeN * mergeNodeN(NodeN * nodeBase, NodeN * nodeNew) {
    if (!nodeNew) return nodeBase;
    assert(nodeBase->type == NODE_N);
    assert(nodeNew->type == NODE_N);
    int newLength = nodeBase->length + nodeNew->length;
    nodeBase->nodes = realloc(nodeBase->nodes, sizeof(NodeSyntax *) * newLength);
    for (int i = 0; i < nodeNew->length; i++)
        nodeBase->nodes[i + nodeBase->length] = nodeNew->nodes[i];
    nodeBase->length = newLength;
    return nodeBase;
}

NodeSyntax * popFirstNodeN(NodeN * node) {
    assert(node->type == NODE_N);
    NodeSyntax * res = node->nodes[0];
    node->length--;
    node->nodes = memmove(node->nodes, node->nodes + 1, sizeof(NodeSyntax *) * node->length);
    return res;
}

NodeSym * createNodeSym(char * name) {
    NodeSym * node = malloc(sizeof(NodeSym));
    node->type = NODE_SYM;
    node->name = name;
    return node;
}

NodeVal * createNodeValChar(char * value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_CHAR;
    node->value.i = value[1];
// TODO (�����#1#): Escape check
    free(value);
    return node;
}

NodeVal * createNodeValInt(int value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_INT;
    node->value.i = value;
    return node;
}

NodeVal * createNodeValDouble(double value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_DOUBLE;
    node->value.d = value;
    return node;
}

NodeVal * createNodeValString(char * value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = STRING_LITERAL;
    node->value.s = value;
// TODO (�����#1#): Escape check
    return node;
}

extern void printfl(int level);

void printNodeN(NodeN * node, int level) {
    printfl(level);
    printf("%s, length: %d", enum_tostring(node->action), node->length);
    for (int i = 0; i < node->length; i++)
        printNodeSyntax(node->nodes[i], level + 1);
}

void printNodeSym(NodeSym * node, int level) {
    printfl(level);
    printf("%s (symbol)", node->name);
}

void printNodeVal(NodeVal * node, int level) {
    printfl(level);
    //printf("%s", enum_tostring(node->action));
    switch (node->action) {
case CONSTANT_CHAR: printf("%c (char)", node->value.i); break;
case CONSTANT_INT: printf("%d (int)", node->value.i); break;
case CONSTANT_DOUBLE: printf("%f (double)", node->value.d); break;
case STRING_LITERAL: printf("%s", node->value.s); break;
default: assert(0);
    }
}

void printNodeSyntax(NodeSyntax * node, int level) {
    if (!node) {
        printfl(level);
        printf("NULL NODE");
        return;
    }
    if (node < 2000) {
        printfl(level);
        printf("%s", enum_tostring(node));
        return;
    }
    switch (node->type) {
        case NODE_N: printNodeN((NodeN *)node, level); break;
        case NODE_SYM: printNodeSym((NodeSym *)node, level); break;
        case NODE_VAL: printNodeVal((NodeVal *)node, level); break;
        default: assert(0);
    }
}
