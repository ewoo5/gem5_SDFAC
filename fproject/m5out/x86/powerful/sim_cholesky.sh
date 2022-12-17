build/X86/gem5.opt --outdir=../fproject/m5out/x86/powerful/CHOLESKY \
configs/example/se.py \
--cpu-type=X86O3CPU \
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
--cmd=../Splash-3/codes/kernels/cholesky/CHOLESKY --options="-p2 ../Splash-3/codes/kernels/cholesky/inputs/tk29.O"