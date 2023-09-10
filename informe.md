# Informe laboratorio 1 - Grupo 21

## Integrantes

- Antequera Ezequiel  
  eantequera404@mi.unc.edu.ar
- Baudino Geremias  
  geremiasbaudino@mi.unc.edu.ar
- Guglieri Juan Cruz  
  juan.cruz.guglieri@mi.unc.edu.ar
- Mollea Maria Agustina  
  agustina.mollea@mi.unc.edu.ar

## Índice

0. Cómo correr el programa  
   0.1 Nuestros tests   
   0.2 Informe de uso de memoria
1. Introducción
2. Modularización  
   2.1 Modulo command  
   2.2 Módulo strextra  
   2.3 Modulo parsing  
   2.4 Modulo builtin  
   2.5 Modulo execute  
   2.6 (EXTRA) Modulo prompt
3. Técnicas de programación

4. Herramientas de programación

5. Desarrollo

6. Conclusiones


## 0. Cómo correr el programa

Para correrlo, se debe ejecutar `make` en la carpeta `src` del proyecto y luego ejecutar el comando `./mybash` allí mismo. El programa se ejecuta en modo interactivo, por lo que se pueden ingresar distintos comandos (ver **help**) o utilizar la combinación de teclas `Ctrl+D` para salir del mismo.

### 0.1 Nuestros tests

Se ha realizado, para nuestro entendimiento y facilitación de corrección, una serie de scripts de pruebas que se encuentran en la carpeta `myTests` y al ser usados imprimen un log que nos brindará información util. Para ser ejecutados/más info. ver el archivo `DOC.md` en la misma carpeta.

### 0.2 Informe de uso de memoria

Para ver el informe/sumario de uso de memoria en el ejecutable mybash se debe dejecutar el comando `make mem` el cual hará un compilado del programa y además la ejecución de la herramienta valgrind.

## 1. Introducción

En este informe se detallará el desarrollo del laboratorio 1 de la materia Sistemas Operativos, el cual consiste en la creación de un bash propio que permita ejecutar tanto comandos simples como comandos con multiples pipes, y que tiene como objetivo principal la incorporación de nuevos conceptos y herramientas brindados por la cátedra, y a su vez fomentar la investigación independiente.

## 2. Modularización

Para el desarrollo del laboratorio se decidió modularizar el código en 8 módulos los cuales trabajan en simultaneo para la ejecución de los comandos ingresados por el usuario.

Se optó por una distribución de tareas modular y no monolítica, ya que esta organización de las funciones nos permite obtener una mayor legibilidad del código, facilita la identificación y corrección de errores.

### 2.1 Modulo command

Se implementaron algunas funciones privadas para facilitar la resolución de aquellos problemas que se fueron presentando durante el desarrollo de los distintos módulos.

### 2.2 Módulo strextra

Se optó por incluir la función strconcate (la cual es util a la hora de concatenar dos cadenas de caracteres) y descartar strmerge ya que con esta última se generaban memory leaks.

### 2.3 Módulo Parsing

El módulo cuenta con tres funciones, dos privadas y una pública, la cual funciona como interfaz del módulo y delega trabajos a las otras.

### 2.4 Módulo builtin

Este módulo interno del bash cuenta con dos secciones de funciones privadas, una de identificación y otra de ejecución.

El mismo entra en acción al ser llamado por el módulo execute.

### 2.5 Módulo execute

El módulo posee unicamente una función pública, interfaz del mismo, la cual toma la estructura conocida **pipeline** y según la naturaleza del comando, realiza lo necesario para poder ejecutarlo correctamente, caso contrario imprimirá un mensaje de error.

### 2.6 (EXTRA) Módulo prompt

## 3. Técnicas de Programación

En general se ha tratado de mantener un lineamiento del código mínimo y limpio, que facilite la lectura, comprensión y mantenimiento del mismo.

En cuanto a librerias externas respecta, se ha tratado de utilizar la menor cantidad posible, usando unicamente aquellas que fueron recomendadas por la cátedra; puntualmente se hizo uso de la libreria **Glib** en la implementación de los TADs pipeline y scommand, y la libreria **string** en la implementación de varios módulos para facilitar el manejo de cadenas de caracteres.

Es destacable el uso de la estructura singly linked list (sList) de la libreria Glib, la cual fue utilizada para implementar tanto funciones privadas como públicas, además de los TADs del módulo command. En esta estructura se almacenan argumentos en el scommand y comandos en el pipeline.

Gracias a esta, se ha facilitado el manejo de la memoria dinámica, administrandosé de forma robusta y segura. Pocas funciones en otros módulos toman memoria dinámica, y en caso de hacerlo, el tratamiento o devolución se hace dentro del mismo módulo, a menos que estemos hablando de uno de los TADs, en donde la memoria se libera después de ser usados.

El código ha sido pensado para que sea fácilmente extensible, por lo que se ha tratado de mantener una estructura modular, que permita la incorporación de nuevas funcionalidades sin necesidad de modificar el código existente.

Por otro lado, se ha tratado de implementar cada linea de código pensando en un mal uso o mala interpretación del mismo que podrian generar baches o intromisiones de un usuario con malas intenciones, por lo que se han implementado funciones de validación y verificación de errores, que permitan evitar estos problemas.

El algoritmo más interesante en cuanto diseño, investigación e implementación llevadas a cabo fue el algoritmo desarrollado en el módulo execute, el cual significó un gran desafío de busqueda de ejemplos, explicaciones, y hasta se ha consultado tutoriales de diversos programadores para ayudarnos a entender el funcionamiento del mismo.

## 4. Herramientas de Programación

Para facilitar algunas tareas de programación se han utilizado algunas herramientas que nos han permitido ahorrar tiempo y esfuerzo, como por ejemplo:

  * **Valgrind**: Herramienta que nos permite detectar errores de memoria, como por ejemplo memory leaks, y tener un mejor control de la memoria dinámica.
  * **GDB**: Debugger que nos permite realizar un seguimiento paso a paso de la ejecución de nuestro programa e identificar errores de programación.
  * **Tokei**: Herramienta que nos permite contar líneas de código y tener un mejor control de la cantidad de código que estamos escribiendo.  
  * **GIT**: Permite llevar un control de versiones de nuestro código y trabajar en equipo de forma colaborativa.
  * **Formatter**: Herramienta que nos permite formatear nuestro código de forma automática, para que sea más legible y siga un estandard.

Empleamos además un estardard a la hora de realizar comentarios en el código, el cual consiste en:

  * Comentarios de una sola línea: Se utiliza para explicar pequeñas porcionees de código.
  * Comentarios de bloque: Se utiliza para explicar el funcionamiento de una función, módulo o una sección de código. Además en este tipo de comentarios, si es que existen, se especifican requisitos y condiciones que debe cumplir el código.

## 5. Desarrollo

Los principales problemas que se han presentado se relacionan principalmente a la falta de conociemientos previos sobre algunos temas trabajados en el proyecto por lo que se tuvo que recurrir en muchos casos a la documentación y clases brindadas por la cátedra y a la búsqueda de ejemplos en internet.

En relación a los puntos estrellas, se pudieron implementar la gran mayoría y a su vez adicionamos algunas funcionalidades extras que no estaban en el enunciado, como por ejemplo:
 * Al hacer help de algún argumento se ejecuta man del mismo.
 * Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples.   
 * Imprimir un prompt con información relevante, por ejemplo, nombre del host, nombre de usuario y camino relativo.  

El problema más grande se presentó a la hora de implementar el módulo execute y entender como trabajaban los procesos hijos, trabajar con pipes, cerrar todos los file descriptors sin cerrar los necesarios, pero gracias a la ayuda de los profesores y a la documentación de internet pudimos resolverlo.
## 6. Conclusiones

Como conclusión, se puede decir que este proyecto significó un gran desafío para nosotros, ya que nos permitió aprender y profundizar en conocimientos que no habíamos trabajado antes, tales como las syscalls (llamadas al sistema) y los pipelines, asi como aprender a distribuir trabajo y trabajar en grupo. 

A su vez se nos brindó la oportunidad de conocer en profundidad el funcionamiento específico de bash, ver su gramática y sus comandos, y poder implementarlos en nuestro propio shell.

También consideramos que este laboratorio nos permitió adquirir habilidades para la resolución de problemas, la búsqueda de información y la comunicación entre nosotros.
