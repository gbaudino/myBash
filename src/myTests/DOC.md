
## Documentacion de nuestros tests

### Cómo correr los tests
Para correr los tests asegurese de incluir en el input.txt los casos que desee probar y luego ejecute make test en el directorio myTests. 

### Requirimientos
Para correr los tests se necesita tener instalado el programa valgrind, asi como tambien tener en el directorio de objects-x86_64 los objetos dados por la cátedra.

### Resultados
Los resultados del test se encuentran en los logs donde se nos indica:
- Si el test fue exitoso o no (si no fue exitoso se nos indica el error).
- La diferencia en la salida con el modulo parsing hecho por la cátedra.
- Los leaks de memoria que se hayan producido.