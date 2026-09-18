# AI Usage Documentation

I used OpenAI's ChatGPT, running the GPT-5.6 Sol model, as an AI assistance tool for this assignment. I used it to help explain concepts, plan the program structure, and review or debug code that I wrote.

For transparency, the prompts I used and the corresponding AI-generated responses are documented below.

## Codex Interaction 1

### Tool
OpenAI Codex

### Prompt
I am working on an individual C programming assignment for a Unix filesystem simulator.

IMPORTANT ACADEMIC INTEGRITY / AI DISCLOSURE RULES:

My instructor allows AI assistance, but all AI-generated text and code must be clearly disclosed, including:
- the prompt I gave the AI
- the corresponding AI output
- what the AI helped with

There should be a file in this repository called:

AI_disclosure.md

You ARE allowed to modify AI_disclosure.md.

As we work on this assignment, keep AI_disclosure.md updated.

For every meaningful assignment-related prompt I give you:

1. Append the exact prompt I gave you.
2. Record a concise but accurate copy/summary of your corresponding output.
3. Clearly identify any code you generated or modified.
4. Briefly state what the AI assistance was used for.
5. Do not remove or overwrite previous disclosure entries.

Use entries like:

## Codex Interaction 1

### Tool
OpenAI Codex

### Prompt
[exact prompt]

### AI Output
[corresponding output or generated code]

### How AI Was Used
[brief explanation]

---

IMPORTANT WORKFLOW RULES:

- First inspect the files currently in this directory.
- Determine which file appears to be the professor-provided starter file.
- Do not assume a filename if you cannot find one.
- Read the existing code before suggesting changes.
- Do not implement the entire assignment at once.
- Work incrementally, one feature or helper function at a time.
- Explain what you are going to change before making significant changes.
- Preserve professor-provided starter code unless modification is necessary.
- Keep the program compatible with Linux and standard C.
- Use gcc warnings when testing, such as:
  gcc -Wall -Wextra -std=c11
- Do not hide compiler warnings or errors.
- Do not modify AI_disclosure.md except for documenting AI usage.
- Do not claim that AI-generated code was written entirely by me.

For now:

1. Inspect the repository.
2. List the files.
3. Identify what looks like the starter C file.
4. Explain what the existing program currently does.
5. Explain what major parts of the filesystem simulator still need to be implemented.
6. Recommend what I should work on first.

Do NOT modify the C source code yet.

You may update AI_disclosure.md to document this interaction.

### AI Output
Inspected the repository and assignment handout. The files are `main`, `main.c`, `AI_disclosure.md`, `Assignment/README.md`, `Assignment/pa0_base.c`, and `Assignment/pa0_documentation.pdf`. The README identifies `pa0_base.c` as the provided skeleton. It defines a filesystem node and root/current-directory pointers, initializes the root node, and enters an empty command loop; the command array and filesystem operations are not implemented. `main.c` is a separate Hello World example, and `main` is its executable. Recommended beginning with safe root initialization and a command-reading loop, then adding one helper or command at a time. No C code was generated or modified. This disclosure entry was appended.

### How AI Was Used
Repository inspection, explanation of the starter code and assignment requirements, and planning the first incremental implementation step. No assignment source code was generated or modified.

---

## Codex Interaction 2

### Tool
OpenAI Codex

### Prompt
One additional workflow requirement for the rest of this assignment:

This directory is a GitHub repository.

After each logical feature is completed and successfully tested:

1. Run git status.
2. Review the changed files.
3. Do not commit binaries, executables, temporary files, or generated files.
4. Commit the appropriate source/documentation files, including AI_disclosure.md.
5. Use a descriptive commit message for the feature.
6. Push the commit to the current GitHub branch.

Before committing and pushing:
- make sure the program compiles
- run relevant tests
- do not push broken code unless I explicitly ask

Do not force push.
Do not rewrite Git history.
Do not delete branches.
Do not use git reset --hard unless I explicitly ask.
Do not modify the remote unless necessary.

If a GitHub push fails, stop and show me the exact error.

Also continue updating AI_disclosure.md for each meaningful assignment-related interaction as previously instructed.

For the work you just completed from my previous prompt:
- if AI_disclosure.md was changed, review it
- if the repository is in a clean, working state, commit that disclosure change with an appropriate message and push it
- do not modify the C source code just to create a commit

### AI Output
Reviewed the disclosure and repository state. The tracked Hello World program in `main.c` compiled with `gcc -Wall -Wextra -std=c11` and ran successfully. The untracked professor skeleton passed syntax checking with its existing warning about an undeclared `strcpy` due to missing `<string.h>`. No C source was generated or modified. Appended this disclosure entry and staged only `AI_disclosure.md` for a documentation-only commit and push to `cpts-360`.

### How AI Was Used
Recorded the requested Git workflow, checked compile status and repository state, and documented this interaction. No assignment source code was generated or modified.

---

## Codex Interaction 3

### Tool
OpenAI Codex

### Prompt
idk i have it tho can u create a .gitignore and a .env then and ill paste it in

### AI Output
Created `.gitignore` to exclude `.env`, object files, `.out` files, and simulator save files. Created `.env` with an `SSH_PRIVATE_KEY_PATH` placeholder. Recommended keeping the SSH private key in `~/.ssh` and putting only its path in `.env`; no private key was read or stored. No C source was generated or modified.

### How AI Was Used
Prepared local Git ignore rules and a credential path template to help configure SSH access. No assignment source code was generated or modified.

---
