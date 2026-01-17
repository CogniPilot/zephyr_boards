# zephyr_boards

This repository contains **out-of-tree** board definitions for the Zephyr RTOS, maintained by **CogniPilot** and **NXP**. It provides support for custom hardware, experimental boards, and specific revisions that have not yet been merged into the upstream Zephyr project.

By adding this repository to your west workspace, you can easily build applications for these boards using standard Zephyr tooling.

## Supported Boards

| Board Name                 | Zephyr Target name      | PCB Design Files / Hardware Source                                                  |
|----------------------------|-------------------------|-------------------------------------------------------------------------------------|
| **NXP MR-MCXN-T1 Hub (Spinali)** | **mr_mcxn_t1** | [CogniPilot/spinali_mcxn_t1_hub](https://github.com/CogniPilot/spinali_mcxn_t1_hub) |
| **NXP MR-VMU-Tropic**      | **mr_vmu_tropic**       | [NXP-Robotics/MR-VMU-TROPIC](https://github.com/NXP-Robotics/MR-VMU-TROPIC)         |
| **NXP MR-NavQ95B**         | **mr_navq95b**          | [NXP-Robotics/NavQ95](https://github.com/NXP-Robotics/NavQ95)                       |

## Usage

To use these boards in your Zephyr application, you need to add this repository to your `west` manifest (`west.yml`) and update your workspace.

### 1. Add to `west.yml`

Open your application's `west.yml` file (usually found in the root of your workspace or application directory) and add the following entries to the `remotes` and `projects` sections:

```yaml
manifest:
  remotes:
    # Add the CogniPilot remote if not already present
    - name: cognipilot
      url-base: https://github.com/CogniPilot

  projects:
    # Add the zephyr_boards repository
    - name: zephyr_boards
      remote: cognipilot
      revision: main
      path: modules/lib/zephyr_boards
```

### 2. Update Workspace

Run `west update` to fetch the new repository:

```bash
west update
```

### 3. Build Your Application

Once the repository is downloaded, Zephyr's build system will automatically detect the new board definitions (provided the project path is known to west). You can build your application by specifying the board name:

```bash
# Example: Building for the NXP NavQ95 board
west build -b mr_navq95b app/your_app_name
```


## Contributing

If you have a board you would like to add:
1.  Fork this repository.
2.  Add your board definition under the `boards/` directory following standard Zephyr board porting guides.
3.  Submit a Pull Request.