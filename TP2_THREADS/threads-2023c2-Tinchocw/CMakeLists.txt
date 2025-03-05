cmake_minimum_required(VERSION 3.27)
project(threads_2023c2_Tinchocw)

set(CMAKE_CXX_STANDARD 14)

include_directories(threads-2023c2-Tinchocw)

add_executable(threads_2023c2_Tinchocw
        threads-2023c2-Tinchocw/client_cliente.cpp
        threads-2023c2-Tinchocw/client_cliente.h
        threads-2023c2-Tinchocw/client_interprete.cpp
        threads-2023c2-Tinchocw/client_interprete.h
        threads-2023c2-Tinchocw/client_main.cpp
        threads-2023c2-Tinchocw/client_protocol.cpp
        threads-2023c2-Tinchocw/client_protocol.h
        threads-2023c2-Tinchocw/common_liberror.cpp
        threads-2023c2-Tinchocw/common_liberror.h
        threads-2023c2-Tinchocw/common_queue.cpp
        threads-2023c2-Tinchocw/common_queue.h
        threads-2023c2-Tinchocw/common_resolver.cpp
        threads-2023c2-Tinchocw/common_resolver.h
        threads-2023c2-Tinchocw/common_resolvererror.cpp
        threads-2023c2-Tinchocw/common_resolvererror.h
        threads-2023c2-Tinchocw/common_sockets.cpp
        threads-2023c2-Tinchocw/common_sockets.h
        threads-2023c2-Tinchocw/common_thread.cpp
        threads-2023c2-Tinchocw/common_thread.h
        #threads-2023c2-Tinchocw/server_main.cpp
        threads-2023c2-Tinchocw/server_aceptador.h
        threads-2023c2-Tinchocw/server_aceptador.cpp
        threads-2023c2-Tinchocw/server_jugador.h
        threads-2023c2-Tinchocw/server_jugador.cpp
        threads-2023c2-Tinchocw/server_sender.h
        threads-2023c2-Tinchocw/server_sender.cpp
        threads-2023c2-Tinchocw/server_receiver.h
        threads-2023c2-Tinchocw/server_receiver.cpp
        threads-2023c2-Tinchocw/server_protocol.cpp
        threads-2023c2-Tinchocw/common_constantes_protocolo.h
        threads-2023c2-Tinchocw/server_monitor_de_queues.h
        threads-2023c2-Tinchocw/server_monitor_de_queues.cpp
        threads-2023c2-Tinchocw/common_printer.h
        threads-2023c2-Tinchocw/common_printer.cpp
        threads-2023c2-Tinchocw/common_wrap_socket.cpp
        threads-2023c2-Tinchocw/server_servidor.cpp
)
