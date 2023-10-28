/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
char transferChar(char bigChar) {
    if (bigChar >= 'A' && bigChar <= 'Z') {
        bigChar = bigChar + 32;
    }
    else if (bigChar == ' ' || (bigChar >= 'a' && bigChar <= 'z')) {

    }
    else {
        bigChar = '_';
    }

    return bigChar;
}

void letter_count(bst_node_t **tree, char *input) {

    bst_init(tree);

    for (int i = 0; input[i] != '\0'; i++) {
        char currChar = transferChar(input[i]);

        int value;
        bool found = bst_search(*tree, currChar, &value);
        if (found) {
            value++;
            bst_insert(tree, currChar, value);
        }
        else {
            bst_insert(tree, currChar, 1);
        }
    }



}

bst_node_t* sorted_array_to_bst(bst_items_t *items, int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    bst_node_t *node = items->nodes[mid];

    node->left = sorted_array_to_bst(items, start, mid - 1);
    node->right = sorted_array_to_bst(items, mid + 1, end);

    return node;
}
/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    if (*tree == NULL) return;

    bst_items_t *items = malloc(sizeof(bst_items_t));
    items->capacity = 0;
    items->nodes = NULL;
    items->size = 0;
    if (items == NULL) exit(1);

    bst_inorder(*tree, items);

    bst_node_t *newTree = NULL;
    bst_init(&newTree);



    bst_dispose(&newTree);
    
    *tree = sorted_array_to_bst(items, 0, items->size - 1);


    items->nodes = NULL;
    free(items);
}