build/RISCV/gem5.opt --outdir=../fproject/m5out/riscv/powerful/RADIX \
configs/example/se.py \
--cpu-type=RiscvO3CPU \
--cpu-clock=3GHz \
--cacheline_size=64 \
--caches \
--l1i_size=32kB \
--l1d_size=32kB \
--l1i_assoc=8 \
--l1d_assoc=8 \
--l2cache \
--l2_size=2MB \
--l2_assoc=16 \
--l3cache \
--l3_size=6MB \
--l3_assoc=24 \
--num-cpus=2 \
--cmd=../benchmarks/riscv/RADIX --options="-p2 -n1048576"