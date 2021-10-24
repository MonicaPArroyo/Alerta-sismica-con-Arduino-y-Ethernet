# Alerta sísmica conectada a una red local
Este es un proyecto realizado con la finalidad de conectar un dispositivo SARMEX a una red local para activar una alerta sísmica en diferentes puntos de un edificio. Se usa el protocolo UDP para enviar desde el transmisor una alerta al receptor. Esto genera que el receptor activa una alarma usando un DFplayer mini y un par de bocinas.

Este repositorio contiene el programa tanto para el transmisor como el receptor (cabe recalcar que de ser necesario pueden crearse varios receptores, y conectarse a la red sin mayor configuración, ya que el mensaje se envía al broadcast de la Red. También se encuentran los diagramas para su implementación.
