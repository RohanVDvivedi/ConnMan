INC_DIR=./inc
OBJ_DIR=./obj
LIB_DIR=./lib
SRC_DIR=./src
BIN_DIR=./bin

CC=gcc
RM=rm -f

TARGET=libconnman.a

CFLAGS=-I${INC_DIR} -I${CUTLERY_PATH}/inc -I${BOOMPAR_PATH}/inc

${OBJ_DIR}/%.o : ${SRC_DIR}/%.c ${INC_DIR}/%.h ${CUTLERY_PATH}/inc ${BOOMPAR_PATH}/inc
	${CC} ${CFLAGS} -c $< -o $@

${BIN_DIR}/$(TARGET) : ${OBJ_DIR}/server.o
	ar rcs $@ ${OBJ_DIR}/*.o

path : 
	@echo "export CONNMAN_PATH=\`pwd\`"

all: ${BIN_DIR}/$(TARGET)

clean :
	$(RM) $(BIN_DIR)/* $(OBJ_DIR)/*