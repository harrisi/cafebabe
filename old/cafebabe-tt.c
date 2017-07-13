// transition table implementation
// (source: https://stackoverflow.com/a/1371654)

#include <stdio.h>

int entry_state(void);
int foo_state(void);
int bar_state(void);
int exit_state(void);

/* array and enum below must be in sync! */
int (* state[])(void) = { entry_state, foo_state, bar_state, exit_state };
enum state_codes { entry, foo, bar, end };

enum ret_codes { ok, fail, repeat };
struct transition {
    enum state_codes src_state;
    enum ret_codes   ret_code;
    enum state_codes dst_state;
};

/* transitions from end state aren't needed */
struct transition state_transitions[] = {
    { entry, ok,     foo },
    { entry, fail,   end },
    { foo,   ok,     bar },
    { foo,   fail,   end },
    { foo,   repeat, foo },
    { bar,   ok,     end },
    { bar,   fail,   end },
    { bar,   repeat, foo }
};

#define EXIT_STATE end
#define ENTRY_STATE entry

enum state_codes lookup_transitions(enum state_codes state, enum ret_codes ret) {
    int i = 0;
    struct transition t;
    while (1) {//i < sizeof(state_transitions) / sizeof(state_transitions[0])) {
        t = state_transitions[i++];
        if (t.src_state == state) {
            if (t.ret_code == ret) {
                return t.dst_state;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    enum state_codes cur_state = ENTRY_STATE;
    enum ret_codes rc;
    int (* state_fun)(void);

    for (;;) {
        state_fun = state[cur_state];
        rc = state_fun();
        if (EXIT_STATE == cur_state) {
            break;
        }
        cur_state = lookup_transitions(cur_state, rc);
    }

    return 0;
}

int entry_state(void) {
    printf("%s\n", __func__);
    return ok;
}

int foo_state(void) {
    printf("%s\n", __func__);
    return ok;
}

int bar_state(void) {
    printf("%s\n", __func__);
    return fail;
}

int exit_state(void) {
    printf("%s\n", __func__);
    return ok;
}
