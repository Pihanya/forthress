# Forthress

> ITMO University
> Computational Technics Chair
> 2nd year of education, 2019
> System-level Software
> Teacher -  Igor Zhirkov ([GitHub](https://github.com/sayon))

---
### Quickstart
- Run interpreter
```sh
$ git clone
$ make run
```

- Run tests
```sh
$ git clone
$ make test
```

### Implemented words

| File | Words |
| ---- | ----- |
| aritchmetic.inc | +, -, *, /, %, < |
| aritchmetic.inc | +, -, *, /, %, < |
| logic_bitwise.inc | and, or, not, lor, land, = |
| data_stack.inc | drop, swap, dup, rot, buf, ., .S, !, @, c@, c!, pushmode_addr, isimmediate, initcmd |
| return_stack.inc | >r, r>, r@ |
| wrappers.inc | read, prints, printnl, parsei, compare, key, emit, cfa, find |
| colon.inc | ', double, >, :, ; |
| system.inc | isbranch, lit, pushlit, unsetbranch, saveword, savetnum, wasbranch, pushmode, buffer, comp_m, init_m, branch, branchifz, docol, ,, create, syscall, warn, bye, exit |
