#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data.h"
#include "bst.h"
#include "hst.h"
#include "input.h"
#include "output.h"

// TODO добавить doxygen
status_t procces_menu_choice(menu_option_t menu_option, bst_node_t **bst_root, hash_table_t **hst);

#endif