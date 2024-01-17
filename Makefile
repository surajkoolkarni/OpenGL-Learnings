SRC=./src
INCLUDE=-I./include
OPT=-O2

Lesson1:lib
	g++ Lesson1.cpp -fPIC $(OPT) $(INCLUDE) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lModel -o lesson1

Lesson2:
	g++ Lesson2.cpp -fPIC $(OPT) $(INCLUDE) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lModel -o lesson2

Lesson2EBO:
	g++ Lesson2EBO.cpp -fPIC $(OPT) $(INCLUDE) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lModel -o lesson2EBO

Lesson3:
	g++ Lesson3.cpp -fPIC $(OPT) $(INCLUDE) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lModel -o lesson3

Lesson4:
	g++ Lesson4.cpp -fPIC $(OPT) $(INCLUDE) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lModel -o lesson4

Lesson5:
	g++ Lesson5.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson5

Lesson6:
	g++ Lesson6.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson6

Lesson7:
	g++ Lesson7.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson7

Lesson8:
	g++ Lesson8.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson8

Lesson9:
	g++ Lesson9.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson9

Lesson10:
	g++ Lesson10.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson10

Lesson11:
	g++ Lesson11.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson11

Lesson12:
	g++ Lesson12.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson12

Lesson13:
	g++ Lesson13.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson13

Lesson14:
	g++ Lesson14.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson14

Lesson15:libModel.so
	g++ Lesson15.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lModel -o lesson15

Lesson16:libModel.so
	g++ Lesson16.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lassimp -lModel -o lesson16

stb_image:
	g++ -shared -fPIC $(OPT) stb_image.cpp $(OPT) $(INCLUDE) -o libstb_image.so 

lib:BufferFactory Camera ElementBuffer GLWindow IBuffer Mesh Model Shader StbImage Texture2D Vertex VertexArray VertexBuffer
	g++ -shared -fPIC $(OPT) BufferFactory.o Camera.o ElementBuffer.o GLWindow.o IBuffer.o Mesh.o \
	 	Model.o Shader.o StbImage.o Texture2D.o Vertex.o VertexArray.o VertexBuffer.o -L. -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lassimp -o libModel.so

# lib:BufferFactory Camera ElementBuffer GLWindow IBuffer Model Shader StbImage Texture2D Vertex VertexArray VertexBuffer
# 	g++ -shared -fPIC $(OPT) BufferFactory.o Camera.o ElementBuffer.o GLWindow.o IBuffer.o \
# 	 	Model.o Shader.o StbImage.o Texture2D.o Vertex.o VertexArray.o VertexBuffer.o -L. -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lassimp -o libModel.so

BufferFactory:$(SRC)/BufferFactory.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/BufferFactory.cpp

Camera:$(SRC)/Camera.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Camera.cpp

ElementBuffer:$(SRC)/ElementBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/ElementBuffer.cpp

GLWindow:$(SRC)/GLWindow.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/GLWindow.cpp

IBuffer:$(SRC)/IBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/IBuffer.cpp

Mesh:$(SRC)/Mesh.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Mesh.cpp

Model:$(SRC)/Model.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Model.cpp

Shader:$(SRC)/Shader.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Shader.cpp

StbImage:$(SRC)/StbImage.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/StbImage.cpp

Texture2D:$(SRC)/Texture2D.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Texture2D.cpp

Vertex:$(SRC)/Vertex.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/Vertex.cpp

VertexArray:$(SRC)/VertexArray.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/VertexArray.cpp

VertexBuffer:$(SRC)/VertexBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(SRC)/VertexBuffer.cpp

clean:
	rm -f lesson* *.o libModel.so
