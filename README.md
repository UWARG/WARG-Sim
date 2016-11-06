# WARG-Sim
2nd Revision of the WARG Autopilot Simulator using Gazebo

##Arduino Instructions

##Gazebo Instructions

For linux usage only.

###Libraries

* [Gazebo 7.1](http://gazebosim.org/download)
* cmake - `sudo apt-get install cmake`

###Compiling

In order to compile the plugins for gazebo the following steps must be completed in the project directory.

1. `cd plugins`
2. `mkdir build`
3. `cmake ../`
4. `make -j4`

Compiled liibraries that work properly should be moved from the build folder into the lib folder for git repository tracking.
When adding a new plugin, it must also be linked in the appropriate \*.world file. The typical structure of the \*.world file is:


`<model name='box'>

      <link name="link" />
        <collision name="collision"/>
          <geometry>
            <box>
              <size>1 1 1</size>
            </box>
          </geometry>
        </collision>
        <visual name="visual">
          <geometry>
            <box>
              <size>1 1 1</size>
            </box>
          </geometry>
        </visual>
      </link>

      <plugin name="model_push" filename="libmodel_push.so"/>
    </model>        
  </world>`

Note that the `<plugin ...` is placed near the end of the model.

##Running

To run gazebo with plugins, one must specify the plugin directory as a environmental variable.

1. `export GAZEBO_PLUGIN_PATH=~/Github/WARG-Sim/plugins/bin`
2. `gazebo ./WARG_WORLD.world --verbose`
