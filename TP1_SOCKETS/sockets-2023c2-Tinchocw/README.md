# Sockets

![img](http://www.plantuml.com/plantuml/dpng/RLF1RjKm4BpdAwoUGAX_mA4IXmeE2AdYR6tNG-i-nnljEa60thzZAyzvBu6SajdTyKns_625jCnZ6bvZWCCubL-EIj7uF1VOYd9KJXY6OI8-qGlStNU3ki-vG7yGuywvFuDpdCZCVKeHkJIWFXyKbE9lKZ_A4iTM4NltljREVM2bcHG5TQQErDU9j8x4R4LdvYZPaqTU8kbbzdoZvPjA4PQqKz51yTpqNBWRkJ2Cr1iIk5IwFHoYTkpsU0N7vnXeatuynQ1B3AARxTQ_im3hnAiJhx1fXXNwZwUlA3z5JpjBJy8dR3_Ma0Ebr3aLlFJFIdI-NTOZZViLVPdn8XjF0ZTzRKSqKITycOsojBL13HtTD7lQhSSaHOw5mZRF8XgGA-A9oqp9huoN-lcQ5yb59L4GzG6UPQGSnDOiyAMe6ruFVgloAy-XuIejO6lPdVJO17JVj51kkGnGyEicsoYLYhuIH_8Ka2YNgtkvrgvp1pzFq0Ng3ddhYlgcUtvY2__gTHGhsfmU_roTkgsFo4PlQ3QXAlrArEUI6lVn_dwBvj1V6lhFnTrNrwGCnwPMwrA7jUSMUK0EyvZ-0W00)

Codigo PlantUML que genero ese diagrama:

```
@startuml
hide empty attributes
hide circle


package "User Interface"  {
  class Client {
    +realizar_movimientos()
  }
  class Interprete{
   +preparar_instruccion_a_enviar()
  }
}

package "Protocol"  {
  class Protocolo_cliente{
    +procesar_select()
    +procesar_dir()
    +instruccion_recibida()
  }
  class Protocolo_servidor{
    +recibir_instrucciones()
    +enviar_respuesta()
  }
}

package "Network"  {
  class Socket {
    +sendall()
    +recvall()
  }

}


package "Game"  {
  class Juego{
    +seleccionar_mapa()
    +gusano_moverse()
    +respuesta_a_instruccion_solicitada()
    +coordenada_actual_gusano()
    }
  class Controlador_de_comandos{
   +controlar_comandos_a_juego()
   +controlar_respuesta_de_juego()
   }
  class Gusano{
    +avanzar_derecha()
    +saltar_hacia_adelante()
    +rotar_hacia_derecha()

    }
  class Server {
    +procesar_juego()
  }
}

class Printer{
   +mostrar_coordenada_actual()
   +mostrar_mensaje_de_error()
  }



Network <-- Protocol
Protocol <-- "User Interface"
Protocol <-- "Game"
"User Interface" <-- Printer
"Game"<-- Printer
@enduml
```
