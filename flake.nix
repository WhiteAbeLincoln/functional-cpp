{
  description = "A functional programming library for c++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/22.05";
    devshell.url = "github:numtide/devshell";
    flake-utils.url = "github:numtide/flake-utils";
    flake-utils.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, flake-utils, devshell, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (system: {
      devShell =
        let pkgs = import nixpkgs {
          inherit system;

          overlays = [ devshell.overlay ];
        };
        in
        pkgs.devshell.mkShell {
          packages = [
            pkgs.clang_13
            pkgs.cmake
          ];
        };
    });
}
# {
#   outputs = { self, nixpkgs, devshell, ... }@inputs:
#     inputs.utils.lib.eachSystem [ "x86_64-linux" "i686-linux" "aarch64-linux" "x86_64-darwin" ]
#       (system:
#         let pkgs = import nixpkgs {
#           inherit system;
#           overlays = [ devshell.overlay ];
#         };
#         in {
#           devShell = pkgs.devshell.mkShell rec {
#             name = "functional-cpp";

#             packages = [
#               pkgs.gcc11
#               pkgs.cmake
#             ];
#           };
#         }
#       );
# }
