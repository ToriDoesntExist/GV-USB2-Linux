{
    description = "A basic flake exposing a derivation";
    
    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    };
    
    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux"; # Adjust for your architecture (e.g., aarch64-darwin)
        pkgs = nixpkgs.legacyPackages.${system};

        packageVersion = "0.1.0";
        kernel = pkgs.linuxPackages_7_2.kernel;
    in {
        packages.${system}.default = pkgs.stdenv.mkDerivation {
            pname = "gvusb2-${packageVersion}-${kernel.version}";
            version = packageVersion;

            src = ./.;

            enableParallelBuilding = true;
            nativeBuildInputs = kernel.moduleBuildDependencies;

            makeFlags = [
                "KBUILD_DIR=${kernel.dev}/lib/modules/${kernel.modDirVersion}/build"
            ];

            installPhase = ''
                mkdir -p $out/lib/modules/${kernel.modDirVersion}/kernel/drivers/media/usb/
                install -D gvusb2-*.ko $out/lib/modules/${kernel.modDirVersion}/kernel/drivers/media/usb/
            '';
        };
    };
}
