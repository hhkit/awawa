How to do soft body physics:

1. The Soft Body 
  - We may have to do some precomputation
    - Pre-existing constraints
      - Distance constraint
      - Tetrahedralize -> compute volume?

2. The Physics 
  - https://matthias-research.github.io/pages/publications/XPBD.pdf
  - Many CPU simulations running around out there 
  - We want to do this on the GPU?
  - CPU version first. Use Diligent, so we can easily slip in the compute shader

3. The rendering
  - Diligent Engine should just be a cmake and build