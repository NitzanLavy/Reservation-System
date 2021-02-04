
#MACROS#
CC = gcc
OBJS_ALL = $(OBJS) $(OBJS_INPUT_CHECKS) $(OBJS_TESTS) $(OBJS_LIST)
OBJS = mtm_escape.o parser.o order.o escaper.o room.o company.o EscapeTechnion.o
OBJS_INPUT_CHECKS = order_input_checks.o escaper_input_checks.o room_input_checks.o \
	company_input_checks.o escape_technion_input_checks.o general_input_checks.o
OBJS_TESTS = order_test.o escaper_test.o room_test.o company_test.o \
	EscapeTechnion_test.o
OBJS_LIST = list_mtm1_test.o list_mtm1.o
MTM_FILE = mtm_ex3.h
TESTS_EXE = order_test escaper_test room_test company_test EscapeTechnion_test
LIST_TEST_EXE = list_mtm1_test
EXEC = mtm_escape
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
LIB_FLAG = -L. -lmtm
TESTS_PATH = ./tests
LIST_PATH = ./list_mtm


#MAIN RULES#
$(EXEC): $(OBJS) $(OBJS_INPUT_CHECKS)
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) $(OBJS) libmtm.a \
	$(OBJS_INPUT_CHECKS)
	
tests: $(TESTS_EXE) $(LIST_TEST_EXE)

clean:
	rm -f $(OBJS_ALL) $(LIST_TEST_EXE) $(TESTS_EXE) $(EXEC)

	
#LIST RULES#
list_mtm1.o: $(LIST_PATH)/list_mtm1.c $(LIST_PATH)/list_mtm1.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(LIST_PATH)/$*.c $(LIB_FLAG)

list_mtm1_test.o: $(TESTS_PATH)/list_mtm1_test.c $(TESTS_PATH)/test_utilities.h \
	$(TESTS_PATH)/../$(LIST_PATH)/list_mtm1.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c $(LIB_FLAG)

list_mtm1_test: list_mtm1_test.o list_mtm1.o
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) list_mtm1_test.o list_mtm1.o

#TESTS EXEC RULES#
order_test: order_test.o order.o general_input_checks.o order_input_checks.o
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) \
	order.o order_input_checks.o general_input_checks.o $@.o

room_test: room_test.o room.o general_input_checks.o room_input_checks.o
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) \
	room.o general_input_checks.o room_input_checks.o $@.o

escaper_test: escaper_test.o escaper.o general_input_checks.o \
order.o escaper_input_checks.o order_input_checks.o list.h
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) \
	escaper.o order.o general_input_checks.o escaper_input_checks.o libmtm.a \
	order_input_checks.o $@.o
	
company_test: company_test.o company.o general_input_checks.o \
company_input_checks.o room_input_checks.o room.o set.h
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) \
	company.o room.o room_input_checks.o company_input_checks.o \
	general_input_checks.o $@.o libmtm.a
	
EscapeTechnion_test: EscapeTechnion_test.o EscapeTechnion.o set.h company.o \
escaper.o room.o order.o escape_technion_input_checks.o  \
room_input_checks.o company_input_checks.o order_input_checks.o \
escaper_input_checks.o general_input_checks.o
	$(CC) -o $@ $(COMP_FLAG) $(DEBUG_FLAG) $(LIB_FLAG) \
	EscapeTechnion.o $@.o escaper.o company.o libmtm.a room.o order.o \
	escape_technion_input_checks.o room_input_checks.o company_input_checks.o \
	escaper_input_checks.o order_input_checks.o general_input_checks.o

#TESTS OBJS RULES#
order_test.o: tests/order_test.c tests/test_utilities.h tests/../order.h \
tests/../general_input_checks.h tests/../$(MTM_FILE)
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c
	
room_test.o: tests/room_test.c tests/test_utilities.h tests/../room.h \
tests/../general_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c

escaper_test.o: tests/escaper_test.c tests/../$(MTM_FILE) \
tests/../escaper.h tests/../order.h tests/../general_input_checks.h \
tests/../list.h tests/test_utilities.h tests/../list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c
	
company_test.o: tests/company_test.c tests/test_utilities.h \
tests/../company.h tests/../room.h tests/../general_input_checks.h \
tests/../company_input_checks.h tests/../room_input_checks.h \
tests/../$(MTM_FILE) set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c
	
EscapeTechnion_test.o: tests/EscapeTechnion_test.c \
tests/../EscapeTechnion.h tests/../$(MTM_FILE) tests/test_utilities.h \
tests/../$(MTM_FILE) set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(TESTS_PATH)/$*.c

	
#OBJS RULES#
order.o: order.c order.h general_input_checks.h order_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

order_input_checks.o: order_input_checks.c order_input_checks.h \
general_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

room.o: room.c room.h general_input_checks.h company.h \
company_input_checks.h room_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
	
room_input_checks.o: room_input_checks.c room_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

escaper.o: escaper.c order.h general_input_checks.h escaper.h list.h \
escaper_input_checks.h order_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
	
escaper_input_checks.o: escaper_input_checks.c escaper_input_checks.h \
order_input_checks.h general_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c	

company.o: company.c company.h room.h general_input_checks.h \
company_input_checks.h room_input_checks.h $(MTM_FILE) set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

company_input_checks.o: company_input_checks.c company_input_checks.h \
room_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

EscapeTechnion.o: EscapeTechnion.c EscapeTechnion.h $(MTM_FILE) set.h \
list.h company.h room.h general_input_checks.h company_input_checks.h \
room_input_checks.h escaper.h order.h escape_technion_input_checks.h \
escaper_input_checks.h order_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
 
escape_technion_input_checks.o: escape_technion_input_checks.c \
escape_technion_input_checks.h escaper_input_checks.h \
order_input_checks.h general_input_checks.h company_input_checks.h \
room_input_checks.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

general_input_checks.o: general_input_checks.c general_input_checks.h \
$(MTM_FILE)
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
 
mtm_escape.o: mtm_escape.c EscapeTechnion.h $(MTM_FILE) parser.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) EscapeTechnion.c parser.c $*.c

parser.o: parser.c EscapeTechnion.h $(MTM_FILE) \
escape_technion_input_checks.h escaper_input_checks.h \
order_input_checks.h general_input_checks.h company_input_checks.h \
room_input_checks.h parser.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) EscapeTechnion.c \
	escape_technion_input_checks.c escaper_input_checks.c order_input_checks.c \
	general_input_checks.c company_input_checks.c room_input_checks.c $*.c	