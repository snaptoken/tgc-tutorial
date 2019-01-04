#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TodoItem {
  struct TodoItem* next;
  char* name;
} TodoItem;

void todo_print(TodoItem* todo) {
  if (todo == NULL) {
    printf("No todos! :D\n");
  }

  int id = 1;
  while (todo) {
    printf("%d. %s\n", id, todo->name);
    todo = todo->next;
    id++;
  }
}

TodoItem* todo_add(TodoItem* todo, const char* name) {
  TodoItem* todo_item = (TodoItem*)malloc(sizeof(TodoItem));
  size_t len = strlen(name);

  todo_item->next = todo;
  todo_item->name = (char*)malloc(len + 1);
  strcpy(todo_item->name, name);

  return todo_item;
}

TodoItem* todo_remove(TodoItem* todo, int index) {
  if (index == 1 && todo) {
    return todo->next; // memory leak!
  }

  int id = 1;
  TodoItem* cur = todo;
  while (cur) {
    if (id == index - 1) {
      if (cur->next) {
        TodoItem* to_delete = cur->next;
        cur->next = to_delete->next;
        free(to_delete->name);
        free(to_delete);
      }
      break;
    }
    cur = cur->next;
    id++;
  }

  return todo;
}

TodoItem* todo_snip(TodoItem* todo, int index) {
  if (index < 2) {
    return NULL; // memory leak!
  }

  int id = 1;
  TodoItem* cur = todo;
  while (cur) {
    if (id == index - 1) {
      TodoItem* to_snip = cur->next;
      cur->next = NULL;
      while (to_snip) {
        TodoItem* to_delete = to_snip;
        to_snip = to_snip->next;
        free(to_delete->name);
        free(to_delete);
      }
      break;
    }
    cur = cur->next;
    id++;
  }

  return todo;
}

int main() {
  char input[1024];
  TodoItem* todo = NULL;
  bool quitting = false;

  while (!quitting) {
    todo_print(todo);

    printf("\ntodo> ");
    fflush(stdout);

    if (fgets(input, sizeof(input) - 1, stdin)) {
      if (input[0]) {
        // Trim trailing newline.
        input[strlen(input) - 1] = '\0';
      }

      switch (input[0]) {
        case '+':
          todo = todo_add(todo, &input[1]);
          break;
        case '-':
          todo = todo_remove(todo, atoi(&input[1]));
          break;
        case '/':
          todo = todo_snip(todo, atoi(&input[1]));
          break;
        case 'q':
          quitting = true;
          break;
        case '\0':
          break;
        default:
          printf("Invalid command.\nAvailable commands: + - / q\n");
          break;
      }
    }

    printf("\n");

    if (feof(stdin)) {
      quitting = true;
    }
  }

  return 0;
}

