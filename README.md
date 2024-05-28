
# IDS Reverse Proxy Using eBPF for Linux Servers

This project sets up an Intrusion Detection System (IDS) reverse proxy for Linux servers using eBPF. The system is designed to monitor, detect, and respond to network intrusions in real-time.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

eBPF is a powerful and efficient way to perform packet filtering and network traffic analysis directly within the Linux kernel. This project leverages eBPF to build an IDS reverse proxy that monitors incoming and outgoing traffic, detects suspicious activities, and can block malicious IP addresses in real-time.

## Features

- **Real-time Traffic Monitoring**: Monitor network traffic in real-time using eBPF.
- **Intrusion Detection**: Detect potential intrusions and suspicious activities.
- **Reverse Proxy Functionality**: Act as a reverse proxy to route and filter traffic.
- **Blocking Malicious IPs**: Automatically block IPs that exhibit malicious behavior.
- **Detecting Port Scanning & DDOS Attacks In Real Time**: Detects common cyber attacks in real time and responds to them

## Requirements

- Linux Kernel 4.18+ with eBPF support
- `bcc` tools (BPF Compiler Collection)
- Python 3.6+
- `iptables` (for managing IP rules)
- `nginx` or `apache` (for reverse proxy functionality)

## Installation

1. **Install Dependencies**:

    ```sh
    sudo apt-get update
    sudo apt-get install -y bpfcc-tools python3-bpfcc iptables nginx
    sudo apt install bpftool
    ```

2. **Clone the Repository**:

    ```sh
    git clone https://github.com/your-username/ebpf-ids-reverse-proxy.git
    cd ebpf-ids-reverse-proxy
    ```


## Configuration

1. **Configure eBPF Program**:

    Customize the eBPF program located in `cmkae-build-debug/ebpf_filename.c` according to your network monitoring needs.


## Usage

1. **Compile and Load eBPF Program**:

    ```sh
    cd ebpf
    clang -O2 -target bpf -c ids_program.c -o ids_program.o
    sudo bpftool prog load ids_program.o /sys/fs/bpf/ids_prog
    sudo bpftool prog attach /sys/fs/bpf/ids_prog msg_verdict
    ```


## Contributing

We welcome contributions! Please read our [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

