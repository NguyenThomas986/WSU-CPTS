from pyeda.inter import *

ZERO = expr2bdd(expr('0'))
ONE  = expr2bdd(expr('1'))

NUM_BITS = 5  # 5 bits -> nodes 0..31

x_vars = [bddvar(f'x{i}') for i in range(NUM_BITS)]
y_vars = [bddvar(f'y{i}') for i in range(NUM_BITS)]
z_vars = [bddvar(f'z{i}') for i in range(NUM_BITS)]


def int_to_bdd(n, var_list):
    # encode integer n as a bdd minterm over var_list (var_list[0] = lsb)
    result = ONE
    for i, v in enumerate(var_list):
        bit = (n >> i) & 1
        result = result & (v if bit else ~v)
    return result

def eval_rel(bdd, a, b, avars, bvars):
    # evaluate a binary relation bdd at (a, b)
    pt = {v: (a >> i) & 1 for i, v in enumerate(avars)}
    pt.update({v: (b >> i) & 1 for i, v in enumerate(bvars)})
    return bdd.restrict(pt).is_one()

def eval_set(bdd, a, avars):
    # evaluate a unary set bdd at node a
    pt = {v: (a >> i) & 1 for i, v in enumerate(avars)}
    return bdd.restrict(pt).is_one()


# --- step 3.1: build rr, even, prime ---

rr = ZERO
for i in range(32):
    src = int_to_bdd(i, x_vars)
    rr = rr | (src & int_to_bdd((i + 3) % 32, y_vars))
    rr = rr | (src & int_to_bdd((i + 8) % 32, y_vars))

even = ZERO
for i in range(32):
    if i % 2 == 0:
        even = even | int_to_bdd(i, x_vars)

prime_nodes = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31}
prime = ZERO
for i in range(32):
    if i in prime_nodes:
        prime = prime | int_to_bdd(i, x_vars)

print("step 3.1")
print(f"  rr(27,  3) = {eval_rel(rr, 27,  3, x_vars, y_vars)}  (expected true)")
print(f"  rr(16, 20) = {eval_rel(rr, 16, 20, x_vars, y_vars)}  (expected false)")
print(f"  even(14)   = {eval_set(even, 14, x_vars)}  (expected true)")
print(f"  even(13)   = {eval_set(even, 13, x_vars)}  (expected false)")
print(f"  prime(7)   = {eval_set(prime,  7, x_vars)}  (expected true)")
print(f"  prime(2)   = {eval_set(prime,  2, x_vars)}  (expected false)")


# --- step 3.2: compute rr2 = r ∘ r ---
# rr2(u,v) = ∃w. r(u,w) ∧ r(w,v)

rr_xz = ZERO
for i in range(32):
    src = int_to_bdd(i, x_vars)
    rr_xz = rr_xz | (src & int_to_bdd((i + 3) % 32, z_vars))
    rr_xz = rr_xz | (src & int_to_bdd((i + 8) % 32, z_vars))

rr_zy = ZERO
for i in range(32):
    src = int_to_bdd(i, z_vars)
    rr_zy = rr_zy | (src & int_to_bdd((i + 3) % 32, y_vars))
    rr_zy = rr_zy | (src & int_to_bdd((i + 8) % 32, y_vars))

rr2 = (rr_xz & rr_zy).smoothing(z_vars)

print("\nstep 3.2")
print(f"  rr2(27, 6) = {eval_rel(rr2, 27, 6, x_vars, y_vars)}  (expected true)")
print(f"  rr2(27, 9) = {eval_rel(rr2, 27, 9, x_vars, y_vars)}  (expected false)")


# --- step 3.3: transitive closure rr2star ---
# iterative fixpoint: tc = tc ∪ (tc ∘ rr2) until stable

def compose(a, b):
    # relational composition a∘b, both over (x_vars, y_vars)
    a_xz = a
    for yi, zi in zip(y_vars, z_vars):
        a_xz = a_xz.compose({yi: zi})
    b_zy = b
    for xi, zi in zip(x_vars, z_vars):
        b_zy = b_zy.compose({xi: zi})
    return (a_xz & b_zy).smoothing(z_vars)

tc = rr2
iteration = 0
while True:
    tc_new = tc | compose(tc, rr2)
    iteration += 1
    if tc_new.equivalent(tc):
        break
    tc = tc_new

rr2star = tc
print(f"\nstep 3.3: fixpoint reached after {iteration} iteration(s)")


# --- step 3.4: evaluate statementA ---
# ∀u. (prime(u) → ∃v. (even(v) ∧ rr2star(u,v)))
# rewritten as: ¬∃u. (prime(u) ∧ ¬∃v.(even_y(v) ∧ rr2star(u,v)))

even_y = ZERO
for i in range(32):
    if i % 2 == 0:
        even_y = even_y | int_to_bdd(i, y_vars)

inner       = (even_y & rr2star).smoothing(y_vars)  # ∃v.(even(v) ∧ rr2star(u,v))
bad         = prime & ~inner                          # prime nodes that fail the condition
exists_bad  = bad.smoothing(x_vars)                  # ∃u. bad(u)
statement_a = exists_bad.is_zero()                   # true iff no counterexample exists

print(f"\nstep 3.4: statementA = {statement_a}")
