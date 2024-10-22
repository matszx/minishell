#  **Minishell**
## Checklist

**General:**
- [X] Display a prompt when waiting for a new command.
- [X] Have a working history.
- [X] Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
- [X] Not use more than one global variable. Think about it. You will have to explain its purpose.
- [X] Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
- [X] Handle ’ (single quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence.
- [X] Handle " (double quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence except for $ (dollar sign).

**Streams:**
- [ ] < should redirect input.
- [ ] \> should redirect output.
- [ ] << should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
- [ ] \>> should redirect output in append mode.

**Env:**
- [ ] Implement pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
- [X] Handle environment variables ($ followed by a sequence of characters) which should expand to their values.
- [X] Handle $? which should expand to the exit status of the most recently executed foreground pipeline.

**Signals:**
- [X] ctrl-C displays a new prompt on a new line.
- [X] ctrl-D exits the shell.
- [X] ctrl-\ does nothing.

**Builtins:**
- [X] echo with option -n .
- [X] cd with only a relative or absolute path
- [X] pwd with no options
- [X] export with no options
- [X] unset with no options
- [X] env with no options or arguments
- [X] exit with no options

ST: 1: TERM_SESSION_ID=w0t0p0:33FDB222-0769-4C92-A424-0190D9E3432B
ST: 3: LC_TERMINAL_VERSION=3.4.16
ST: 11: TERM_PROGRAM_VERSION=3.4.16
ST: 24: __CF_USER_TEXT_ENCODING=0x0:0:0
