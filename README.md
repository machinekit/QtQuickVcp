#QtKnobs (formerly Knobs 'n' Dials)
QtKnobs is a Qt and QML based Library/Plugin which provides different types of Knobs.

Screenshots:
- ###### Types

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/alltypes.png)

- ###### Percent View

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/percent.png)

- ###### Specify Max values

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/maxvalues.png)
  
- ###### Different sizes

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/sizes.png)
  
- ###### Customize

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/custom.png)

#List of properties
* size
* value
* maximumValue
* percent
* readOnly { true, false }
* mode { Knob.Normal, Knob.Percent }
* style { Knob.Pie, Knob.Arc, Knob.Needle }
* pieType { Knob.Flat, Knob.Curve  } (**when style = Knob.Pie**)
* needleType { Knob.Point, Knob.Round, Knob.Groove  } (**when style = Knob.Needle**)
* color (**knob color**)
* backgroundColor (**back dial color**)
* foregroundColor (**front dial color**)
* borderColor
* textColor
* meter { true, false } (**simple meter**)
* pieMultiColor (some fun)
 
#Requirement
Qt >= 5.3

#Building
* Clone
* Run qmake && make

The compiled library (libQtKnobs.so on .nix systems) would be in directory **Knobs** with **qmldir**.

#Use
Copy directory **Knobs** to your project location and include in it your .pro file.
For eg:
```
unix:!macx: LIBS += -L$$PWD/Knobs/ -lQtKnobs

INCLUDEPATH += $$PWD/Knobs
DEPENDPATH += $$PWD/Knobs
```
Then to make the engine to search for this module, add the path where the **Knobs** directory is using *addImportPath*. 
For eg. If the directory **Knobs** is at location */home/ashish/KnobsTest* then,
```
QQuickView view;
view.engine()->addImportPath("/home/ashish/KnobsTest");
view.show();
```

Additionally, to make QtCreator aware of it (and to get rid of the annoying **QML module not found** during import) add the path in .pro file,
```
QML_IMPORT_PATH = /home/ashish/KnobsTest
```

#Examples

* A simple Knob:
  
  ```
  import QtQuick 2.0
  import Knobs 1.0
  Knob {
    id: myKnob
  }
  ```

  ![ScreenShot](https://raw.githubusercontent.com/ashish157/Knobs-n-Dials-QML/5c5e347b649606533a95330b9cafb3b4eb4b8155/QtKnobs/screens/default.png)

   You can find more examples in QtKnobs/examples/main.qml

#License

MIT

#Contact
Feel free to contact me for any questions at ashishd157 at gmail.com.
