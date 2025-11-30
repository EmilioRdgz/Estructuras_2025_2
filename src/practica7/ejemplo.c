char* infixPostfix(char *expresion){
  if(!estaBalanceada(expresion)){
    printf("La expresión no esta balanceada\n");
    return NULL;
  }
  int longitudExpresion = strlen(expresion);
  int cantidadParentesis = contarParentesis(expresion);
  int longitudReserva = longitudExpresion - cantidadParentesis;
  int i,j;
  char *postfix = calloc(longitudReserva + 1, sizeof(char));
  //Convertir Expresión A Postfix
  //Usando Pila
  Pila pila = {NULL,-1,0,&imprimirCadena,&liberarCadena};
  for(i = 0; i < expresion[i] != '\0'; i++){
    char c = expresion[i];
    // si es espacio continuar
    if(c == ' ')
      continue;
    // si el caracter actual es un Operando, entonces meter directamente a expresión Postfix
    if(esOperando(c)){
      postfix[j++] = c; ///agregar a la cadena
    }
    else if(c == '('){ //si es parentesís ingresar el parentesís a la pila
      char *nuevo = malloc(sizeof(char));
      *nuevo = c;
      push(&pila,nuevo);
    }
    // si caracter actual es ')'
    else if(c == ')'){
      while(!vacia(pila)){
        char *cima = (char*) pop(&pila);
        if(*cima == '('){
          free(cima);
          break;
        }else{ //de lo contrario agregar la cima a la exporesión poatfix
          postfix[j++] = *cima;
          free(cima);
        }
      }
    }

    else if(esOperador(c)){
      while(!vacia(pila)){
        //hacer peek de la cima para conocer el dato
        char *peek = (char*) pila.cima->dato;
        if(*peek == '('){
          break;
        }

        //usar la función de comparar comparar el dato actual con la cima de la pila para saber que operador es mayor
        //y saber si sacar de la pila o meter
        int comp = compararPrecedencia(&c,peek);
        if(comp <= 0){
          char *nuevoMayor = (char*)pop(&pila);
          postfix[j++] = *nuevoMayor;   //agregar a la expresión Postfix el operador que se encontrar en la cima de la pila
          free(nuevoMayor);
        }else{
          break;
        }
        //Meter a la pila el cáracter actual
        char *nuevo = malloc(sizeof(char));
        *nuevo = c;
        push(&pila,nuevo);
      }
      else{
        printf("Carácter Inválido\n");
        free(postfix);
        eliminar(&pila);
        return NULL;
      }
    }

  }
}