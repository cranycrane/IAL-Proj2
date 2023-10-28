/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bool search = false;
  bool finish = false;

  while (!finish) {
    if (tree == NULL) {
      finish = true;
    }
    else if (tree->key == key) {
      //printf("Found node %c with value: %d\n", tree->key, tree->value);
      finish = true;
      search = true;
      *value = tree->value;
    }
    else if (key < tree->key) {
      tree = tree->left;
    }
    else if (key > tree->key) {
      tree = tree->right;
    }
  }


  return search;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bool finish = false;
  bst_node_t **currentNode = tree;

  while (!finish) {
    if ((*currentNode) == NULL) {
      (*currentNode) = (bst_node_t *) malloc(sizeof(bst_node_t));
      if (*currentNode == NULL){
        exit(1);
      }

      (*currentNode)->key = key;
      (*currentNode)->value = value;
      (*currentNode)->left = NULL;
      (*currentNode)->right = NULL;
      finish = true;
    }
    else if ((*currentNode)->key == key) {
      (*currentNode)->value = value;
      finish = true;
    }
    else if (key < (*currentNode)->key) {
      currentNode = &(*currentNode)->left;
    }
    else if (key > (*currentNode)->key) {
      currentNode = &(*currentNode)->right;
    }
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (!tree || !*tree || !target) {
        return;
    }

    bst_node_t *rightmostParent = NULL;
    bst_node_t *rightmost = *tree;

  while (rightmost->right != NULL) {
      rightmostParent = rightmost;
      rightmost = rightmost->right;
  }

  target->key = rightmost->key;
  target->value = rightmost->value;

  if (rightmostParent) {
    rightmostParent->right = rightmost->left;
  } 
  else {
      // Pokud nejpravější uzel je kořen podstromu (tj. *tree)
      *tree = rightmost->left;
  }


  free(rightmost);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bool finish = false;
  bst_node_t **currentNode = tree;

  if (*currentNode == NULL) {
    return;
  }

  while (!finish) {

    if ((*currentNode) == NULL) {
      finish = true;
    }
    else if (key < (*currentNode)->key) {
      currentNode = &(*currentNode)->left;
    }
    else if (key > (*currentNode)->key) {
      currentNode = &(*currentNode)->right;
    }
    else if (key == (*currentNode)->key) {
      if ((*currentNode)->left == NULL && (*currentNode)->right == NULL) {
        free(*currentNode);
        *currentNode = NULL;
        finish = true;
      }
      else if ((*currentNode)->left != NULL && (*currentNode)->right != NULL) {
        bst_replace_by_rightmost((*currentNode), &(*currentNode)->left);
        finish = true;
      }
      else if ((*currentNode)->right == NULL) {
        bst_replace_by_rightmost(*currentNode, &(*currentNode)->left);
        finish = true;
      }
      else if ((*currentNode)->left == NULL) {
        bst_replace_by_rightmost(*currentNode, &(*currentNode)->right);
        finish = true;
      }
    }
  }


}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t *stack = (stack_bst_t *) malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    exit(1);
  }
  
  stack_bst_init(stack);

  while (*tree != NULL || !stack_bst_empty(stack)) {
    if (*tree == NULL) {
      if (!stack_bst_empty(stack)) {
        *tree = stack_bst_top(stack);
        stack_bst_pop(stack);
      }
    }
    else {
      if ((*tree)->right != NULL) {
        stack_bst_push(stack, (*tree)->right);
      }
      bst_node_t *deletedNode = *tree;
      *tree = (*tree)->left;
      free(deletedNode);
    } 
  }

  free(stack);

}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {

  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    bst_add_node_to_items(tree, items);
    tree = tree->left;
  }


}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = (stack_bst_t *) malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    exit(1);
  }
  
  stack_bst_init(stack);
  
  bst_leftmost_preorder(tree, stack, items);
  while (!stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack, items);
  }
  
  free(stack);
  /*

  if (stack == NULL || tree == NULL) {
    exit(1);
  }

  stack_bst_push(stack, tree);
  while (!stack_bst_empty(stack)) {
    bst_node_t *currentNode = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_add_node_to_items(currentNode, items);

    if (currentNode->right) stack_bst_push(stack, currentNode->right);
    if (currentNode->left) stack_bst_push(stack, currentNode->left);
  }

  */




}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  } 
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = (stack_bst_t *) malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    exit(1);
  }
  
  stack_bst_init(stack);
  
  bst_leftmost_inorder(tree, stack);
  while (!stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, stack);
  }
  
  free(stack);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {

while (tree != NULL) {
  stack_bst_push(to_visit, tree);
  stack_bool_push(first_visit, true);
  tree = tree->left;
}


}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  bool fromLeft;
  stack_bst_t *stack = (stack_bst_t *) malloc(sizeof(stack_bst_t));
  stack_bool_t *boolStack = (stack_bool_t *) malloc(sizeof(stack_bool_t));
  
  if (stack == NULL || boolStack == NULL) {
    exit(1);
  }

  bst_leftmost_postorder(tree, stack, boolStack);

  while (!stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    fromLeft = stack_bool_top(boolStack);
    stack_bool_pop(boolStack);
    if (fromLeft) {
      stack_bool_push(boolStack, false);
      bst_leftmost_postorder(tree->right, stack, boolStack);
    }
    else {
      stack_bst_pop(stack);
      bst_add_node_to_items(tree, items);
    }
  }


}
