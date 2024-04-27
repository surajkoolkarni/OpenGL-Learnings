MODEL=./Model
MODEL_VIEWER=./ModelViewer
STB_IMAGE=./stb_image
GLAD=./glad

INCLUDE= -I$(STB_IMAGE) -I$(GLAD) -I$(MODEL)

OPT=-O2

ModelViewer:stb_image glad lib
	g++ $(MODEL_VIEWER)/ModelViewer.cpp -fPIC $(OPT) $(INCLUDE) -L./ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lassimp -lModel -o viewer

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

lib:stb gladLib BufferFactory Camera ElementBuffer GLWindow IBuffer Mesh model Shader Texture2D Vertex VertexArray VertexBuffer Error
	g++ -shared -fPIC $(OPT) BufferFactory.o Camera.o ElementBuffer.o GLWindow.o IBuffer.o Mesh.o Error.o \
	 	Model.o Shader.o Texture2D.o Vertex.o VertexArray.o VertexBuffer.o -L. -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglad -lstb_image -lassimp -o libModel.so

stb:
	g++ -shared -fPIC $(OPT) $(INCLUDE) $(STB_IMAGE)/stb_image.cpp $(OPT) $(INCLUDE) -o libstb_image.so 

gladLib:
	gcc -shared -fPIC $(OPT) $(INCLUDE) $(GLAD)/glad.c -o libglad.so

BufferFactory:$(MODEL)/BufferFactory.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/BufferFactory.cpp

Camera:$(MODEL)/Camera.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Camera.cpp

ElementBuffer:$(MODEL)/ElementBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/ElementBuffer.cpp

GLWindow:$(MODEL)/GLWindow.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/GLWindow.cpp

IBuffer:$(MODEL)/IBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/IBuffer.cpp

Mesh:$(MODEL)/Mesh.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Mesh.cpp

model:$(MODEL)/Model.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Model.cpp

Shader:$(MODEL)/Shader.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Shader.cpp

Texture2D:$(MODEL)/Texture2D.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Texture2D.cpp

Vertex:$(MODEL)/Vertex.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Vertex.cpp

VertexArray:$(MODEL)/VertexArray.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/VertexArray.cpp

VertexBuffer:$(MODEL)/VertexBuffer.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/VertexBuffer.cpp

Error:$(MODEL)/Error.cpp
	g++ -c -fPIC $(OPT) $(INCLUDE) $(MODEL)/Error.cpp

clean:
	rm -f lesson* *.o libModel.so
