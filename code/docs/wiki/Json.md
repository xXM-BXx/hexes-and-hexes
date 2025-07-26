# Json Formatting
My Engine uses ```.json``` to load setups for several different classes.

Beyond my engines usage, you can of course define you own structure for e.g. save games.

## Animations

The ```AnimatedSprite.loadAnimations()``` expects this format:
```json
[
  {
    "animationName": "myAnimation",
    "looping" : true,
    "frames": [
      {
        "atlasCoordinate": [0, 0],
        "duration": 0.15
      },{
        //...
      }
      
    ]
  },{
    //...
  }
]
```
You can also load a single animation with ```AnimatedSprite.loadAnimation()```, which then only expects one element instead of a list.
## Atlas Textures
The ```AtlasTexture.loadAtlasTexture()``` expects this format:
```json
{
  "defaultOffset": [0, 0],
  "defaultSize": [1, 1],
  "atlasItems": [
    {
      "atlasCoordinates":[0, 0],
      "size": [3, 3],
      "offset": [24, 28]
    },
    {
      "atlasCoordinates":[3, 0]
    },
    {
      //...
    }
  ]
}
```
You may leave size and offset, if you don't need it, but even if you always define them you still need to provide default values.
This format is mainly useful with irregular shaped atlas items.
## Terrain Sets
The ```AutoTileTerrain.loadNeighborRelation()``` expects this format:
```json
{
  "defaultTextureId": 0,
  "relations": [
    {
      "neighbors": [],
      "textureId": 1,
      "atlasCoords": [
        0, 0
      ]
    },
    {
      "neighbors": [
        0, 5
      ],
      "atlasCoords": [
        1, 0
      ]
    },
    {
      //...
    }
  ]
}
```
Once again you need to provide a default id. The texture ids are defined by you in the HexGrid.
The Neighbors mean, where there should be a tile with the same terrain for the which texture.
0 is on top, and the numbers are the 6 directions clockwise.
## Bitmap Font
The ```BitmapFont``` requires this format in it's constructor:
```json
{
  "lineHeight": 16,
  "defaultSize": [6, 10],
  "defaultOffset": [0, 2],
  "defaultAdvance": 7,
  "glyphs": {
    "A": {
      "atlasPos": [0, 0]
    },
    "B": {
      "atlasPos": [6, 0]
    },
    "\u0020": {
      "atlasPos": [54, 60]
    }
  }
}
```
You will once again need to provide default values, but can override them with ```size, offset, advance```.
Also don't forget to set [Space] with ```\u0020```.