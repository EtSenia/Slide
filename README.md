# Proyecto: Generador de Presentaciones con Lenguaje de Descripción

Este proyecto es un generador de presentaciones visuales basado en un lenguaje de descripción estructurado. Utiliza un lenguaje de gramática personalizado para definir las posiciones, tamaños, colores e imágenes de las diapositivas.

## Descripción

El generador permite crear presentaciones usando un lenguaje de marcado específico. Este lenguaje describe las diapositivas, sus elementos de texto e imágenes, y cómo deben ser presentados visualmente. Los usuarios pueden definir la apariencia de cada diapositiva mediante transformaciones como posicionamiento, tamaño, filtros visuales y colores.

Este proyecto está diseñado para facilitar la creación de presentaciones personalizadas de forma sencilla y legible, utilizando un formato estructurado que describe los diferentes elementos visuales.

## Características

- **Diapositivas personalizables**: Cada diapositiva puede tener múltiples elementos, incluidos texto e imágenes.
- **Transformaciones visuales**: Soporta operaciones como filtros de opacidad, brillo, desenfoque, y otros efectos visuales.
- **Definición de estilos**: Permite ajustar las propiedades de texto como fuente, tamaño, color, y alineación.
- **Sintaxis clara**: Usa un lenguaje de marcado fácil de leer y escribir, similar a JSON pero adaptado para presentaciones visuales.

## Estructura del Proyecto

El proyecto está compuesto por varias secciones de código que definen el comportamiento y la estructura de las presentaciones. Cada diapositiva (`slide`) puede contener:

- Un fondo (`background` o `bg`).
- Imágenes (`image`) con filtros aplicados.
- Texto (`text`) con propiedades de estilo.

### Ejemplo de uso

Aquí tienes un ejemplo básico de cómo se ve una diapositiva:

```plaintext
slide {
	bg {
		bg= #22211E;
	}
	image {
		position=80%, 50%;
		width= 40%;
		height= 100%;
		src= "Buildings.jpg";
	}
	text {
		content= "Teoría de compiladores";
		position= 30%, 25%;
		width= 800px;
		align= center;
		font-size= 60;
		color= #fff;
	}
}
```

### Componentes

- **Textos**:  
  Pueden definirse con diferentes propiedades como `font-size`, `font-family`, `align`, y `color`.

- **Imágenes**:  
  Se definen con su fuente (`src`), tamaño (`width`, `height`), y filtros (`filter`).

- **Fondos**:  
  Se pueden especificar colores sólidos o imágenes.
