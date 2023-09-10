## Informe 
- Muy bueno, pueden seguir con este formato de informe para el resto de la materia. Me gustaría si a la sección de modularización la resignifican para que sea más informativa.

## Repo 
- Parece todo bien

## Código
- En general muy bien. Claro y prolijo.
- La función principal en `execute.c` es muy larga. La redirección se puede escribir en una función aparte.
- `define`s para separadores
- Quizás tratar redirecciones in y out en una misma función?
- `camelCase` vs. `snake_case`
- Problema en el tratamiento especial de comando inicial y final (mezcla de competencias)
- Limpiar `argv` en caso de falla de `execvp`

## Funcionalidad
- Testeo 100%
- Hay algún memory leak
- Comportamiento inesperado cuando se pasan builtins dentro de un pipeline `ls | cd`.
- Comportamiento en `ls | wc > salida`
- Puntos estrella
