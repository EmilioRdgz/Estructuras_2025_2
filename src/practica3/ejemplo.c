/*
//inicializar pilas
Pila b = {NULL,-1,0,&imprimirCadena,&liberarCadena};
Pila  = {NULL,-1,0,&imprimirCadena,&liberarCadena};

while(!vacia(b)){
  push(b,pop(a));  //ingresar cima de a -> b
  while(!vacia(a)){
    if(pila->comparar(peek(a),peak(b))<=0){
      push(c,pop(a)); //si el dato cima de b es más grande a dato cima de a; meter a pila c
    }
    else{
      push(b,pop(a)); //de lo contrario dato más grande entonces meter a pila b
    }
    if(vacia(c)){
      printf("Pila ordenada correctamente\n");
      imprimirPila(b); //pila b es la ordenada
    }
    else{
      push(a,pop(b));
      push(a,pop(c));// Regresar datos a pila a
    }
  }
  *a=b;
}

 */