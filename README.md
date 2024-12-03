## Offensive-Development

This is the repository for the 12/3 meeting on Offensive Development for SIGPwny's Purple Team.

In here, you'll find various basic techniques that assist with both speed and stealth when doing pentesting that can get you started on Offensive Development. This should be a good starting point for anyone looking to bypass antivirus solutions.

# C#
The C# folder contains a local inject loader using the Lazarus chain (HeapCreate->UUIDFromStringA->EnumSystemLocales), as well as ConfuserEx with a build script that builds and automatically obfuscated the result.

# C
The C folder contains a similar local injector that contains some useful snippets for a no-CRT binary, including runtime linking and syscalls with HellHall. It's a mix of my own work and various other code snippets from C5pider, Maldev Academy, and VX-API.

# PowerShell
The PowerShell folder contains a handful of snippets to get you started, as well as Chameleon for some easy automated obfuscation.

# What should you work on?
Everything in this repository is able to get past the latest Defender with ease. Don't expect to get past CrowdStrike Falcon or any half-decent EDR with any of these techniques - there's intentionally a lot of room for improvement left.

For example, consider implementing:
- A different local inject chain for both loaders that's less signatured
- A remote inject chain that can bypass some EDRs (e.g. ThreadlessInject + ModuleStomping + CaroKann)
- API hashing in the C# example, and removing some of the obvious delegate names
- Process hollowing with command line spoofing, PPID spoofing, and MapViewOfSection injection
- More robust sandbox detection
- Integration with client-side attacks (like VBA phishing / DotNet2Jscript)
