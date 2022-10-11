Import("env")

env.AddCustomTarget(
    name="nanopb",
    dependencies=None,
    actions=[
       " (cd proto; "
       "python3 ../.pio/libdeps/leonardo/Nanopb/generator/nanopb_generator.py  "
       "--strip-path --output-dir=../src settings.proto)"
    ],
    title="Nanopb generate step",
    description="Rebuild .c/.h files from .proto"
    #always_build=True
)