#-------------------------------------------------------------------------------
# medartprovider.py
#-------------------------------------------------------------------------------

import wx, zlib, cStringIO
from wx import ImageFromStream, BitmapFromImage

class MedArtProvider(wx.ArtProvider):
    def __init__(self):
        wx.ArtProvider.__init__(self)

    def CreateBitmap(self, id, client, size):
        bmp = wx.NullBitmap
        data = None
        if client == wx.ART_OTHER:
            if id in medArts:
                data = eval(id.lower())
                stream = cStringIO.StringIO(zlib.decompress(data))
                bmp = BitmapFromImage(ImageFromStream(stream))
        return bmp

medArts = ['MED', 'OPEN', 'CLOSE', 'CLOSE_DISABLED', 'SAVE', 'SAVE_DISABLED', 'CONVERT', 'CONVERT_DISABLED',
           'UNSPLIT', 'SPLIT', 'ABOUT', 'FLOPPY', 'HARDDISK', 'CDROM', 'DIR', 'FILE', 'DB3', 'XML', 'TABLE', 'INDEX']

med = (
"x\xda\xeb\x0c\xf0s\xe7\xe5\x92\xe2b``\xe0\xf5\xf4p\t\x02\xd2\x02 \xcc\xc1\
\x04$'L\xcc0\x03R\xcc\xc5N\x9e!\x1c\x1c\x1c\xb7\x1f\xfa?\x00rg{\xba8\x86hL\
\x9c\x1at\x90\xf7\x88\x01\x87\xf7b\xbdCy7\x9b\xcc\xa3+\x16\xe4L\x0c}\x97x\
\xd2=\xb3z\xf9\x0fA\xd6\x16C5\xcbgsr\xd9kW\xef\x12b\xf8\xc3\xd1\xb9\xf9H\xda\
\xb3w\xe5\xe7$\x16\x0b1hH\x0b\xee\xec\xde=\xfbf\xd4k\xef\x84\xad\xb7\xd2'^\
\x9f\xb8\xc2\xee\x18C\x80\x88\x11\xdf\xda\xd0\xd2\xfe\xb7\xa6\xb9\xe7\x95wZ\
\xaf>yf\xf9\xccX\x11\x13\xff\x9b\x93\x04\x04M\xee.\x95\x10d\x98\xb1W;\xe4\
\xf2\xcd\x8f\xb3\xefv\xb3k\x9e\x90\xd17\xf9\xb6G\xb5\x89\x7fIz\xd0\xa6\xad\
\xc7|T\xff\x1c\xfd\x96\xff\x97a\xc5\x96t\xd5\x88\xe2k\x1c@g2x\xba\xfa\xb9\
\xacsJh\x02\x00\xc0:Xs"
)

open = ('x\xda\x01\x0e\x02\xf1\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\
\x10\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\
\x08\x08\x08\x08|\x08d\x88\x00\x00\x01\xc5IDAT8\x8d\xa5\x93\xbdkTA\x14\xc5\
\x7f\xb3\xd9b\xc1"\xb7\x94\x14\xe1a!\xcb\xa6\xc8\x8a\x16\xeb\x07,6\x8b\x08\
\x92"\x7f\x80A\xecl"6A\x08XXX\xc9\xb3\t\x08B"\x08i7M\x08\xa41\xdd\x12cx\x0b\
\x06\x83\x10\x99\xed\xec\xbc\xab\xcd\xed\xc6b\xe6\xednXA!\x03\x8fyw\xe6\xdcs\
\xce\xbd3\xe3\\e\x86\x8b\x8c\xea\xff\x80N\xcf\xce\xc2\xf6\xceG0@\x84\xdb\x0b\
st\xda7\x1d\x00\xae2\xf3\xcfoe-\x0f{\x07\xc7\xa1\x8c_\xe4[\xe1\xe8\xcb\xb7\
\xb0\xb2\x96\x07\xe7\xbf\x9f\x84\xe2p\x7fJu\xfe\xca\x02\xd7[\xf7\xdc\xdf\x1c\
}\xfa\xfc5\xbc|\xbb\x1bK(\x0e\xf7i\xd6\x05\xc9\x04\x80Y\x14\x0cv\xf6N\xe8\
\x1f\xed\x86\xc5\x1b\xf7\xa7H\xca\xe4\xd8\x03\x03\xc98\x97\x0c\xb0\xd4\xf2\
\xbc\xef\xc1\xe6\xc6z\x00\xc0@\x01Ly\xfa\xfc\x99\x1b\x13\xa4\xcd\xd9\xda8\
\x19\xf5\x80\xf2\xb0UL\xd9\x7f\xd3\x95sqU\xcb\xbf\x94|\xd0S@Pk\xa6\xf5\x88\
\xd0\xb4/5\xd8|\xbd\x1eb\xacT\xb1\xa4\\\x13\x06\xde\xa3&,\xb5F\xb4\xd3#\x11\
\xf6\x0bO\xff\xe7\x9d\x89{`J\xe1\x05\x91\x124A\x92\xd40\x18\xaa\xa2?\x94\xed\
\xde\x1c\x8fW;\xb1\x89\x982P\x00\xa1]\xd7\x98[\x1b\x97\x85)\xc3\xd2\xa9)[=\
\xa5\xde\xb8\xcb\xd5\xc65W)\x15\n/d\x97\'\xd4\x13\x18U\x86\x1agT\xf1\xa7\x1e\
\xef\xe1V\xe7Qj\xa2\x81On\x17\xb3\x04Lu\x0e\xad\x8cAMQU\xba^\xb84\xdf\xa6\
\xdeh:\x80*(\xdegd\xe2\xe9w\xf3Q\xd9\xf1\xcce\xd4}5AM\xf0\x1e\x96\x9f<`\xe3\
\xdd\x07\x00\\\xfej5`\x1a\x81\xa6\xd1i9\xab\xa2\x16{\xa2\t#\xc0\xe0\xd7\xef\
\xd1Er\x17}\xce\x7f\x00\\\x14\xe6\xd2\x0b}#\x98\x00\x00\x00\x00IEND\xaeB`\
\x82\xa7\xcb\xee\x90'
)

close = (
'x\xda\x01\x8e\x01q\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01EIDAT8\x8d\xa5\x93-O\x03A\x10\x86\x9f-\'p\
\xd7_@\x00\xd7\x90@\x13\x08\t$\xa4U\x88&\x14\x0c\xbf\x00}\x06\x85\xc3"\x9b\
\xd4 \x10\xc8&\x98\x064I\x9b`\x10\x84\xe2\x08\xeadQ\xbc\xe7\xd6\x1d\xe2\xf6>\
\x96\xb6\x02\x18\xb3\x93\xd9yv\xde\x99\xdd5\xa6\xb6\xc4\x7f,\xf8-\xd0\xeaFi\
\xee\x8f\xef\xfb\xa6\xf6\xd7\xca\xd1\xd9Q\xa6\xe0\xeb\xf3#\x1d=>x\x9b\xdb{\
\x1dV\xd6\x1af\x11|\x11\x9dr\xd5\xbfc\xfc\xf4\x9c\x9a\xe1\xa0\x97\xb6\xdb\
\xab\x84\xcb\x90\x00(f4\x82\xf5\xcd\x06[;\x9d\x85\x87\xbc\xbc\xbe\xa5\xe7\
\x977\x98\xe1m/\x9d\x97 \x00[x\x9em4\x0f\xd8\xdd?4\xadn\x94\x06\x00\xc7\'\
\xcdy8\xd8|u\xbe\x15\x89Do\x10\x03\xd9\x10\x03\xfd\x80\x12 D\x0epq\x89\xc4\
\xad\xf1\xfb\x04h\x17TPTq\x16ZeP\x0e[\x07\xdbL\x89\xac\x90J\xa6\xb8\xc6\xc4\
\x93^\x81s%*UUxjyrX\xed\xb5\x80\xfd\xfe\xb1B\x96Y\x05I\x15\xf4`\xcd\xc0\x90\
\xb5Q\x1c\x90\x07\xa1L\x9c\x81%w\xadB\xb6\xee)\x08\x8a\x8a\x8a\xd14\x06\x1b\
\x97\xc3\xca\x07\xa7\xb2\x88l\xddS\x10\x00\xf4\xae\'n\xba \xd5\xd1\xd4\xf9Vs\
\xde\x91\x1f0\xff\xfd\xce\xdf\xd8\x11\xd8e25\xaa\xe0\x00\x00\x00\x00IEND\xae\
B`\x82h\xfa\xab\''
)

close_disabled = (
'x\xda\x01X\x01\xa7\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\x0fIDAT8\x8d\xa5\x93\xb1n\x830\x14E\xaf)\
\x92?\xa4eg`l\xe0+\x92L\xac\xfc\n\xff\xc0\xe4\xb5-\x03s\xe66c\xdb\x91\x11\
\xa9\xad\x04\xdbc{\x9e\xdc!rd;\xd0\xaa\xcd\x95\x10\x96\x9f\xcf\xbb\xefZ Dt\
\x83k\x14\xff\x15\xd8\xef\xb6\xc6\xae\x1f\x1e\x9fD\xf4_\xe7\xb2,O\x13|}~\x98\
\xc3\xe1\xe0\x15\xf3<\xc7\xed]"\xd6\xe0\xaa\xaa\xd04\r\x8e/\xcfF(\xa5L\x9a\
\xa6\x90RBk\r"\xc20\x0cH\x92\x04\xf7\x9b|\xb5\xc9\xfb\xdb\xab\xa9\xeb\x1aB)e\
\x96\x0e0\xb3\xf7v\x95e\x196y!\xf6\xbb\xad\x89\x01\xa0(\x8aU8\\\xcf\xf3\x8c\
\xb6m\x01\x9c.1\x0e\x9d\xb4\xd60\xc6,\x82\xcc\x8c\xbe\xef=\xb38\x1cq\r\xb6""\
/\xd6\xf9;\xb0\xcen\x0c\x17\x0e\xe3XEvCJ\xe9\x1ddfh\xad/\xf6\x98\x19D\xe4O\
\xe0\xe6v\xe1\x10\xb4\xcen\x83(\xcc\xfb\x13\xbc\xd4 \xb6\x85i\x9a0\x8e\xe3\
\xb9\xe8^Vhr\x11\xa1\xeb:\x10\xd1\xe2\xf3\x9b\xc4\xb5\xbf\xf37\x04Z&\xf7\x02\
%\x04\xe4\x00\x00\x00\x00IEND\xaeB`\x82g@\x94H'
)

save = (
"x\xda\x01\x95\x01j\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01LIDAT8\x8d\xa5\x93\xa1n\xc30\x10\x86?\xa7\
\x05a\xf5\x1blp\xac\x95F\x06\r\x07\xf3\x08\xed\x13\xf4\x15\n\x07\xb3\x8dt\
\xac\xd2\xc8\xd8\x82\x86+M\x9a\xaa\xa1\x94\x95L2\x1c\x99ta.\xea@\xec\xc4\xc9\
Z\r4R\x94\xf8|\xf7\xff\xff\xfd\xf6)\x95\x0c8\xe7\x19\x02\xbc\xbe\xfd\x1c\x00\
p>\xea@\x1c\xe0\xa4\xfe\xf6c~}\xffp\xad\x86aS\xeb\x11\xe5\x0e\xe6\xf3\x18\
\x7f\xd4a\xab\xf60\x9dZt\r\x01@\xd2\xc9p\xd5I\xa9\xd5\xbe\xde\x17\x07\xe5\
\xaeh \x12\xa0\x91)N\xfe-\x16\x84\xecv\x01>7\x89{o\xfa=U,x/\xfa-\x84@OA\xbfX\
\x9c\xf8\x97.\x80\x1cQp\xac8>\x89@\xd6\x9c\x02\x0e\xb2l\xccl\xb6Eh\x93\xdb\
\xff\x9a9\xbb\xba\xf4qZ\x00q-\xbb1cD\xaa\x88)f\x976\x1e\x03\xd4\x89\x17^n\
\xc5z\xb3\xe8:\x99\x82N5\xe2L{\xb9\xe2\x16$\xa8\x08\xcc@\xf1\x9231\x86\xd5\
\xb2\x80\x14\xf2e\x8e\xd8\xbffw<\x88\x91'\xc6`n2t\x1a+\x10\xb4'\xd4\xae\x07 \
\xae\x02'\xac6%|\x97\xe4wu\x1b\xd6ZH\xc1\x96k k\xe6 \xd8\xa8T2\xe0\xe3\xdd\
\x1e\x9e\x9e\xbf\x10\x91\xee]\x08\x83\xd3\xf8\x14\x16u|\xfb\xf9\xa8\xd4\xb9\
\xe3\xfc\x0b\x97\xdc\xee\xe2b\xfa\xde_\x00\x00\x00\x00IEND\xaeB`\x82?\x89\
\xb0\xe8"
)

save_disabled = (
'x\xda\x01\x81\x01~\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x018IDAT8\x8d\xa5\x931n\x021\x10E\x9f\x17\x8a\
\xed\xd8\x92\x9b\xe0\x8e\x96k$\xe10\x94P\xd2mI\x9b\xa4\xa1\xa0\xa4M\xb2I.@I\
\xe9\x95\xbc\xd2 \x8dD*[6,\xa4\xc0\x955\x9e\xff\xbe\xe7[6\xa6\x18\xf0\xc8\
\x1a\x02|}~\x9c\xd3\xa2\xaa\xa2\xaax\xefQ\xd5\x9b\xb5\xa7\xe7\x173\x0c\xa2\
\xd1h\xc4\xf1x\xc4Z{\xd3MDX\xadV\x11\x00P\xa4\r]\xd7\xdd\x15\x9fN\'D\x84\xfd\
~\x1f!E8\x04\xf0\xde\xff+v\xce1\x9f\xcf\xa3\xa6\xb8l\xbc\'\xee3*\xd2\xc2%\
\xe0R\xec\x9c\xc39\x97\xf5\x15@\x9c\'=\xe8\x13\x87\xba\x88D\xd3a\x00\xa8*\
\xd3\xe9\x94\xf5z\x9d5\x87}p\xb6\xd6""\xd14\x03\x00L&\x13\xda\xb6\x8dN\xe9\
\xdb{\xef\xb3," 4\xa8*m\xdbR\xd7\xf5U\x98UU1\x1e\x8f# @\xb2\x1b\x04g\x80\xba\
\xae\xb1\xd6\xb2\xd9l\x00X.\x97\xd9hY\x88\x01\x92\x92\xad\xb5\xccf3\xca\xb2\
\xa4,K\xaa\xaa\x8a9\xf4f\xd0u\x1d"\xc2v\xbb\xa5i\x1a\x16\x8b\x05\x00\x87\xc3\
\x01\x80\xa6i\xe2\x08\xe9-\x8c)\x06\xfc\xfe|\x9fw\xbb\xdd\xd5\x1b\xa7\x1f\'u\
\r\xfb\xd7\xb7wc\x1e\xfd\xce\x7f\x9c\xd2QHb\xfa\x8e\x13\x00\x00\x00\x00IEND\
\xaeB`\x82za\xa6\xf1'
)

convert = (
'x\xda\x01E\x02\xba\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\xfcIDAT8\x8d\xa5\x93\xbfk\x13a\x18\xc7\
\xbfwq\xe8\x98)7\xa4\x84\x0cBE\x07OtPQ\xb8I+.\xe9? \xe7\xe6f\xc6\x8e\xf9\x03\
\x84\xdc Z\x88\x98+\x086\x83\xf6\\4\x1dbO\x04\xe9P\xca\t\xa6Ii\x82/\xf9\xd1\
\xbcw-\xf2\x8e\xb7}\x1d\xd2\xa4\x9e\xd7\xad/<\xbc\xf0}\x9f\xe7\xf3\xfc\xe0y5\
M\xcf\xe0"\xe7\xd2yb\x18F\xdc\xef\x0f\x81\x18\xc0\xc2T\xcbP\xc7\xd2\xe5<\x0c\
#\xa7\x9d\x0b\xa8\xbe\xfd@\x00\xb8\x7f}\t\x86\x91\xd3\x06\xc31\xc5 B_\xfc\
\x81i\x16\xa1\xa4\xc2\x8f\xf6\x10a\x18\xd20\x8c3\x88\xa6g\xe6\xe65\x03:k\x1e\
\xff\xd5\xa2\xe3\x13:\xaeG\x15+*\xa5\xe8z>\xa3\xe3\x93\xb9\x8f>\xcb,\xc4\x80\
*V\x10R%\xda1\x0cCC\x0c(\x05(\xa5`^)\xe2s\xeb\xd7\xfc]\x07\x80\xfa\xc7\x16\
\xb7\xf7\x0eO\xa5$`/\xe8\x10\x00\x94T\x10rv+H)\t\x00Z\xbb\xd3g\xad\xf1\x1dB\
\nd\x17\xb2\xa8W\x9f\xe3\xd3\xd6O\xdc\xbd\xb5\x08#gh\xefj\xeb\xbcv\xf3\x1e<?\
\x00bu\x86\x8f\x81\'+w\x923\xd0\xf4\x0c\xca\x15\x87$\xe95\x03*\xd7\xa5\xb0K\
\xb4\xad\x12wv\xdb\xd4\xf4\x0cd\x18\xb1\\q)\xa4\xa2\xbd\xea0\x05\xe8\xfd\x16\
\xb4\xcbS\x88\xeb\xf9tM\x8b\xff\xfb\xec\xec\xb6i\xaf:\x94a\x94\x06\x84\x8d\r\
\xfa\xb6\xcd\xd2\xb3\xca\x14\xb2\xe1s"e\n2\xb3\xc4"\xc9\xdaKN\xeakP\n0E\x80\
\x95Xa\xb3^\xc5z\xe3\x1b\xc6\xe3\x11\xf3\xf9\xc5\xc4\x12%\xf6@\xbey\xcd`\xd9\
\xa2w\xdb\xa2\x935y\xb0\xb5\xcd\x83^\x9fV\xa9L\x92t\\\x9f\xe3\xa3I\xaa\x92\
\xd3\xe0W\x89\xe0n\xf3\xeb\xdc\xb1{\xd8\xa3\xb9l\xcf\xdb\x19\x8d\x8f\x98\x02\
\xf8\xa6I\xb7h\xd2\xc9\x9a\xec|i\xa5\xb2\x0cG\xe3D%\xa9\x19\x04b\xda\xce\xc3\
\xf7/p\xf5\xf1\x83T\x9f\x85BA\xdb\xefvx\xe3\xd1S\x94,+9\x82\x8b~\xe7\xbfMU8\
\'\x8a\x9b\xc4\xbc\x00\x00\x00\x00IEND\xaeB`\x82O\xd6\x0e\xb1'
)

convert_disabled = (
'x\xda\x01O\x02\xb0\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x02\x06IDAT8\x8d\xa5\x93\xb1n\xdaP\x14\x86\
\x7f\x9b<\x00\xca\x00HmZ\xb1!\xb1\xb1 \xb5}\x00#\x06<\xc2F6\x1e\x00\t/Hl02\
\xb24\xf1F\x18Zx\x80&\x98\x01a\x86Xj+Z\x88HD\x83\rv\x9c\xe1\x0e\x0cl\x7f\xa7\
\xb8q\xc9\x96+\x9d\xe5\xdc\xa3\xef\xfc\xff\xb9\xe7J\x92\x1c\xc1k\xce\xd1K\
\xc9\x07\xcf\xe5\xed\xedm(\'\xcb2\x92\xc9$b\xf1\x84\xf4"\xe0\xfc\xec3\x01 \
\x93\xc9 \x16OH\xf6\xfa\x9e\x8e\xe3`\xb3\xd9 \x95JA\x08\x81\xd9l\x86\x07\xcf\
e\x08"\xc9\x91 \x0c\xc3\xa0\xae\xeb|\x9e\xf3}\x9f\xba\xaes\xb7\xdbQ\x08\xc1\
\xc1`@\xdf\xf7\x83\x1a\xf9\xa9\xf3\xfd\x9f\x15\x85\x10p]7$\xfd\xa9\xdbn\xb7\
\x83\x10\x02\xa9T\n\xe3\xf1\xf8\x9f5\x00\xe8\x7f\xfd\xc2\xeb\xebk\x00\xc0~\
\xbf\x0f\x01~\xfe\xf8N\x00\x10B\x04\xe1\xba.<wK\x00\x90\x16\x8b\x05\xfb\xfd>\
V\xab\x15\xa2\xd1(\x9a\xcd&F\xa3\x11\xd2\xe94b\xf1\x84\xa4\x9f\x9f1\x9b\xcd\
\xc20\x0c\xec\xf7\xfbP\x83|>\x1f\x9e\x81$G\xd0h4H\x92\x86a\xd04Mv:\x1d\xaa\
\xaaJ\xcb\xb2(\xc9\x11x\x9e\xc7V\xabE\xd7u\xa9i\x1a\x0f\x00www\xd44\x8d$9\
\x18\x0cX\xa9T\xf8\x7f\x8deY\xd44\x8d\x9e\xe7\x1d\x02L\xd3d\xbb\xddf\xa5R\t \
\xdb\xed\xf6\x00\x12\xbc\xc2\xf3\x81\r\xaf.i\x18\x06l\xdb\x86m\xdb8==E\xa1P\
\xc0t:\x85c\xaf\xf9\xd2\xd2\x05\x8btu\xf9\x8d\xa6i\xe2\xf1\xf1\x11\xcb\xe5\
\x12\xf5z\x1d\xc7\xc7\xc7(\x95J\xe8v\xbb\xe8\xf5zp\xec5\xdf\xbc=\tm"$9\x82\
\xe1p\xc8V\xab\xc5j\xb5JUUi\x9af \xf9\xe6\xe6\x86\x8a\xa2\x04v\x1c\xc7\t\xd9\
\x81$G\xa0i\x1a\xcb\xe52UU\xe5d29\xf0k\xdb6\x8b\xc5"I\xf2\xe2\xe2"t\x7f\x04\
\x00\xf3\xf9\x1c\x00P\xab\xd5\xf0\xe1\xe3\xa7\xb0D\x00\'\xef\xdeK\xbf\x7f\
\xcd\x98\xcb\xe5\xa0(J\xd8\xc1k\xbf\xf3_G\xf96\xe7\xa3\xdf\x134\x00\x00\x00\
\x00IEND\xaeB`\x82\xa8\x97\x0e\xc0'
)

unsplit = (
'x\xda\x01\t\x02\xf6\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\xc0IDAT8\x8d\xa5\x93\xb1kSQ\x14\x87\xbf\
\xfb\xde\x13\x13\x954\xd2\xd6F\x10\x94d\x12DE\xc5\x7f\xc5\xa58\xa8C\x9dU\x04\
\x17\x8b\x88N\xc5EE\xc5MH\x87\xaeM\x97\xe2\xea\xa6\x11\x04E4\x1a\x1c\xe4A\
\xb1\x06\xac\x89\xef\xddw\xceqx/\x11I\x14J\x7f\xf3\xbd\xdf\xf9~\x07\x8esA\
\xc8N\xe2\x96\x9a/\xcd\xf6T\xd9\xec\x1bf\x80*\xa6\x8a\xa8`j\xa8)&\x82\x9aa"\
\x88*S\xbb=\xd3\xfb\xca\\\x9d?\xeb\xa2~4E\xa5\xb4\x97\xb9\xd2\xf6&w\xde\xbec\
\xad\xb5j\xd1 \x83\xc1\xe6\xd6\xb6\xd5\x07\x19\xc4qLdb\\\x98O\x01\x87\xc3\
\x11\x10ps\xe5\x06\xe7O^\xe2\xc4\xc1S\xec/\xcf\x80S\xa2 \xfa\x0bp\xf1n\x17\
\x80\xc0L\x01\x10\x15\x9a/\x9e\xe5\xf4~\x82d\x82\x99\x11\x85\xc1D\x03Q\x01 \
\xc2\x0c\x03\x1e\xad?\xe0[o\x83\xeeF\x97JX!\xcb\x04\xacxm\xe3\x00\xcdr@ *<Y\
\x7fH(\x01G\xa6\xeb<m=\xa6\x14\x96A@\xd5\x8a\xff\xe3\x04\x95\xdc<h\x7f] K\
\x84\xfa\\\x83\x99\xea,\xa7\x8f\x9e!M=I\x9a2\xac\xe7p\xe3\x80a\x85\xe3\x07\
\xee\xf3\xe3\xe7\x12\xaf\xde\xb7\xf1\xde\xf3\xa1\xf3\x91\xc6\xa1\x06\xe2ed0)\
\xaa\x85A\xe6=\xd7\xce]\xa7\xf7\xbdG\xe7\xd3g\x9a\xb7\x96i\xbfy\x8d37\x9al6\
\x0e\xca|\x96\x03\xac\xe8r{\xe1\x0e\xf5Z\x1d\x80\xe4\x97\'\xf5\xfeO\x057^\
\xc1\x86\x06C\x95(\x8cX\xbc\xbc\x08\x80O\xfchI\xff\xacPX\xb9+\xf7\x9e\xdb\
\x96\xeeBEPQD\x04UE\xbcGU\xf3\x1bP\xc1\xa4\xb8\x0bUT\xa1qx\x96c\xd5/8\x17\
\x84\xac\xb5V-\x8e\xe3\xffN\x9c\x94Z\xad\x96\x03v\x92\xdf)u\x00<3\xe7S\x01\
\x00\x00\x00\x00IEND\xaeB`\x82\xfbv\xe8='
)

split = (
'x\xda\x01<\x02\xc3\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\xf3IDAT8\x8d\xa5\x93?h\x14Q\x10\xc6\x7fow\
sg\x94@<\x90;5 \x18\x05\x83)-R\x88\x8d\xd8Y*X\x88\x85V\x8a\xd8\xa5\x12\x0bAI\
\xa1\x95\x08\x9a\xc2N\x0b\xb5\x0b\xc95\xa2\xc5\x15ilT\xc4?XD=\xd4\r\x97D!\
\x97\xcd\xed\xed\x9b\x19\x8b\xdd\xfcC\x9b#S\xbd7\xcc\xfb\xbe\xf9\xbe\x99\xe7\
\\\x10\xb2\x9dpO\xeb\xaf\xed\xa7\xaf\xd0j\x1bf\x80*\xa6\x8a\xa8`j\xa8)&\x82\
\x9aa"\x88*\x95\xfe\x8c\xd1}e\xce\x9d\x1es\xd1\x97\x95\n\x03\x03\xbb\xa8Vzc\
\x9e\xfd\xf8\x89\xfa\xcc\xb4EIf$K\xed\x9e[O2#\x8ec"\x13\xe3\xca\xf9\x103X\
\xe9\xb6\xf9\xf6g\x8e\x17su\xee\x8e=\xfc\xef\xc34]\xa5\\\xee\xe7\xd2\xc4W\
\x00\x023\xfd\xa7\xc8w=\x00wfo\x13\xff^&^H\xf8\xd1j3\x17/\x13\x86\xb9\xe9\
\xa2\x92\x03\xe4\xce\x81a\x80\xc3\x0c\xbai\x06\xc0\xe7\xf9\x0fL4\xc6\x89\xca\
\x01\xa5R\xc4\xce\xbeh\x9dD}\x01 *X\x914S\xbc\xf7\x0c\xed8\xc0\x85\xe7g\x18,\
\x0f\x82\x197\xa6\xae\xd2\x17\x86D\xa5`\x03@\xb4\x90\xa0\x86[\xcb\x1a\x88\
\x17\xd24\xe5\xf8\xe1\x13\x1c\xac\x0e3<t\x88\x8e$\x9c\xbdw\x92R\xb8\xb13ZH\
\x88\xd6\x0ef\x96\xcfZ\x8d\xd5N\x87\xc6\xbb\x06\xb5\xdd5ZK\x0bX\xdb15\xfe\n\
\xddd\x97\x16\x97\xc0g\xd9z\xd2\xcc\xe8v3\xe6\x17\xe7\xb9~\xea&o\xde\xbfee\
\xb1\xc3\xa3\xcbOp\x81c\xf3\xd2\xfa\xcc\x17\x12D\xb7x ^h\xfej2\xb2\xf7(G\xaa\
\xa3<\xbe\xf6\x0c\x9c#\x0c\x02B\xe76\xc8\x8a\x0e"\xdd\xd4\x97\xa9\xe1\xbd\
\xa7\x93\xa4\x00\xdc\xbf\xf8`\xeb|\xc3\x90,\xcb%k1=wk\xf2\xa55\xdb}\xa8\x08*\
\x8a\x88\xa0\xaaH\x96\xa1\xaa\x85/\x82I\xf1/TQ\x85c#{\xd8_\xfa\x8esAH}f\xda\
\xe28\xa6\xd7\xa8\xd5j9\xc0v\xe2/\xdbF \x8au\xdd\xa2\xb1\x00\x00\x00\x00IEND\
\xaeB`\x82\xaf\xcb\x04\x8e'
)

about = (
'x\xda\x01\xc5\x02:\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x02|IDAT8\x8d\xa5\x93Ml\x8bq\x18\xc0\x7fo\xbb\
\xc4|t{\xcd\x12\r\xaa\x9d\xcd6\xb3\x8f\x12\x9b\xe1\xd0\xcd\x81\x04I\xe3\xb0\
\xb8H|\xc4\xc1m\xb8q s\x11n\xbbq\x91\xbaLvp(\x12\x84\xb0\x8a\x8fV"\xba\xd2Ym\
\xd5v\xb1N|\xfe_\xd2\xee\xed\xba\xecqx%\xc5\xe2\xc2sz\x9e\'\xcf\xf3\xcb\xf3\
\xa9i6;\xff#e\x7f:\xa6>\x15$99\xcd\xdbl\x9e\xc9\x0fy\n\xc5\x19\xaa+\xecx\x9c\
\x0bYWSA\xadK\xd7~\x8d\xd7~\xad \x96\xcc\xc9H*G~v\x96\xee\rKY\xbaPCDP\xa6p\
\xed~\x8a\x99\xdc4\x9b\x9a\xaa\xd9\xb1\xd5\xa3\xcd\x03\xc4\x929y\x9d\xce\xb3\
\xbdC\xe7\xd5\x98Fh\x14\xa2i+\xc8\xeb\x01_\xa3\xc1jw\x05\x81\x81\xa7t\xb68\
\xd9\xddUoA4\x9b\x9d\xf7_fe\xf0\xc1g\xf9\x9c\x9f\x93\xc0\x90H\xf7I\x91\xc0m\
\x11e\x8a\x88\x88\xa4\x95\xc8\xf1\x8b"\x81\xdbJR_\xe7\xe4\xc4\x85\xbb\x12\
\x1f\xff \x9a\xcdn\xcd \x995q\xe8\x1a/\xdfh\\\xb9\x03^7\x1c\xd8\t\xc1\x08\
\x04\xc3\xe0\xef\x843\x07ao\x1f\xe8\xfa7ZZW\xf2\xe8y\x06\x00\x1b@jj\x1aoM\
\x05\xa1Q\xc0\x84\xb4\x82\xfe \xf4\r\xc2P\xc2 \x146\xa8\\\x00z9\x04\x87\xd2l\
\xabw\x11\x1e\x9e(m!\xfb1\xcf\xe22\x9dh\x02\x14\xa02\x90\xceXzW\x03\x9c9ZI(a\
\x10\x8dF\xa1\x1c\xce\x1d\x81\xf8X\xb6\x04(\x16g\x11\x01eZ\x15(\x0c0\xc1\xdb\
\x00\x97{+\t\x86\x0c\x8e_\x8a\xa2P\xe8\x00"\x14\x8b\xc5R\x0b\xcb\x1c6\x8c\
\xc2\x1c^\xb7\x812\rP\nL\x85\xcfcm\xe1P\x7f\xda\x82\x03^\x8f\x8e2\xbeQ\xeb\
\xaa*\x01\xdc\xcer^\xbf\xfb\x8e\xcf\x0b\x98\n\x85B\x99\x8aPBqv0\x03X@L\xf0\
\xfb<\xc4\xc7\r:Z]%@\xa3\xc7A,\x96e}\x9d\x83^\xbf\x05\x01\x85\xaf\x01N\xefs\
\xff\xb4\xe1\xd8>\x0f[6.a\xe0F\x84\xed\x9dk\x7f?\xa4{\xe1\x8c<~1\xc9\xfe\x9e\
v\x1e>\x9b \x18QDG\xadDo\xa3\x8e\x7f\xb3\xce\xb6f\x07\xa7\xce_gWW\x13\x87{\
\xb6j\xf3N\xf9\xd6\xc3\xa4\xdc\x8f\xa4hjv\xd2\xb6j\x11+\x96WY\xc35\x0c\xe2\
\xe3\x06WoF\xd8\xd3\xbd\x9eC?\x93\xe7\x01\x00\xe2c\xef\xe5\xc9\x8b\t\xc2\xc3\
\x13\x8c\xbc\x99\xa2P,\xb2f\x95NG\xab\x8b\xee\xcdu\xb4\xb7\xd5\xfc\xfd\x99\
\xfeE~\x00?s#Wg\xf1A\xa4\x00\x00\x00\x00IEND\xaeB`\x82\xe6\xbf?\x9c'
)

floppy = (
'x\xda\x01\xc5\x02:\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x02|IDAT8\x8d\x95\x91\xcbJ\x1b\x01\x18F\xcf\
\xc4\xa1\x93\x84\x98\xa4\xd1\x80\xf1\xd2I\xf0\x02\x05[\xec\xa0\xbb\x80\xd6\
\xcb\xbat\xeb+\xe8C\x14\xdfC)Xp\xa1\xd2\x85E\xe9F\xa3)T\xbc\xa0\x11\x91\xc4X\
\x8d\xb7h\x92I\xed`&qB\x86\xe9"h\xa1I\x17\xfd\xd6\xe7?\xffM\x10l\r\xfc\x9d\
\xeb\xab_\x96\xaa\xaa\xc8\xb2\x8ft:M \x10 \x9b\xcd2=}\xc1\xe8\xa8\xc5\xe0\
\xe0 \x92\xdd!\x00\x885\xd5\xc0\xd1Q\x85\x99\x99\xefLN\x06\x01\xb8\xb8\xb8\
\xa0\xad\xad\x8d\xcb\xcb,\x9f>}\xc5\xe3\xf1`<\x94,\xc9\xee\x10l\xf5\x04\x86a\
\x02P,\x16\x89\xc7\xe3\x98\xa6\x89(V{\xe5r9\xca\xe5\xf2\x13[w\x02\xb3ZO\xa1P\
 \x14z\xcd\xd5\x95\xc6\xf6v\x12Ij\xaaa\xff)\x90\xa4&\xe6\xe6.\xe9\xea2)\x95N\
\xd1\xf5"\x86\xd1]\xc3\xd6]\xc14\xc10\xf2LL\x0c\x12\x0c\xda\x19\x1f\x7fK8\
\x1c\xae\x87\xd6\nRg\xa7\x96i\n\x00\xac\xaf\'1\x0c\x1f\xbd\xbd\xbd<<\x94\xea\
\nD\xbdpo\xa9\xaa\x8a\xa6i8\x1c\x0e\xdcn7 <\x01~\xbf\x9fL&\xc3\xdd]\x06\x97K\
\xc7noA\xd34,\xcb\xaa\nvww\xe9\xec\xec$\x14\n!\x08\x02\x95J\x85r\xb9z\xc5\
\xe1a\x0f\xba~\xca\xe6\xe6\x1dCC\xafho?!\x95\xea"\x1a\xfd\xc6\xedm\x96\xbb\
\x9fyKX^^\xb6\xecv;\x92$\xe1\xf5zinn\xe6\xf0\xf0\x98RI#\x18\x0crpp@*\x95B\
\xd34\xfc\xfe\x00\x85\x82\xc1\x99\xb5\xc3s\xf3\x05\xed\xde\x006UU\x11E\x11\
\xa7\xd3I>\x9f\'\x16\x8b\xe1r=#\x10\x08\xb0\xb2\xb2\xc2\xf1q\x92\xb3\xb3\x14\
\x8b\x8b\x9f\xb9\xbe\xbedvv\x9a\xf7o>\xd0(\xc8\x9c\x9f\x9fc\x1b\x19\x19!\x1a\
\x8d\x12\x8f\xc7q\xbb\xdd\xb4\xb4\xb4P\xa9T\x98\x9f\x9fG\x14%\x96\x96\xbe\
\xb0\xb0\xb0@.\x97A\x10\\LM}$\x12Y\xe0\xf0p\x03\x87\xc3\x81 \xd8\x1a\xf8q\
\x92\xb4\xb6\xb6\xb6\xd8\xd8\xd8@\x96edYfmm\x8d\xb1\xb1w\xec\xec\x1c\xd0\xdd\
\xfd\x92\x9b\x9bkb\xb1Mt]\xc5\xef\xf7\xd3\xd7\xd7\x87\xa2(U\x01\xc0\xe37\xf6\
\xf7\xf7\x89D"\xa4\xd3i\x06\x06\x86H$NH&w\xf1\xf9|ttt\xa0(\n\xfd\xfd\xfd\xb4\
\xb6\xb6\xe2t:\xff\x08\x1eS\xd4\x0b\xd6\xfd\xfd=\x89D\x82\xd5\xd5U\xf6\xf6\
\xf6\xe8\xe9\xe9!\x1c\x0e\xa3(\n^\xaf\x97F\xb7\xe7\xe9\xcf5\x82\xff\xcdo\xfd\
7\x1cE\xc56\x17,\x00\x00\x00\x00IEND\xaeB`\x821\rMq'
)

harddisk = (
'x\xda\x017\x02\xc8\xfd\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\xeeIDAT8\x8d\xc5\x93Mk\x1aa\x14\x85\x9f\
\xd7\xf81\x8e\x0e4\x06\xc9\xcc\x10\x17\xf1\x83\xeaBW\x82\x90E {\xfb\xfb\xb2\
\xcco\xe8&\xbb\xc4\x14\xdc\xa4&\xd0N\x88\x01\x83\x08Nc\xcd(\x89\xe3;\x89\x1f\
Q\xa7\x8b\xd2\x12\xd2R\n)\xf4\xec.\xdc\xf3p\xb9\x9c#D`\x8d\xd7(\xf0*\xf7\xbf\
\x00\x04\x9f\x0f\x03\xe7\xd6\xbf\xbf\xbfg8\x1c\xa2i\x1a\x89D\x82^\xaf\x87\
\xa2(\x00\xe4\xf3yB\xe1\x88x\xee\x11\xb6m\xfb\x8b\xc5\x82\xeb\xebkj\xb5\x1aR\
J\xaa\xd5*;;;\xa8\xaa\xcah4BJ\x89\xae\xebX\x96\xc5d2!\x16\x8bQ*\x95P\xa2\xaa\
\x086\x9bML\xd3$\x9b\xcd\xa2i\x1a\xa6i\xb2Z\xad~{\xee\xf6v\x8enw\xc4\xf1\xf1\
{\x8e\x8e>\xd0\xbb\xf9\xe2\x8b\x83\x83\x03\x7f\xb9\\b\x18\x06\x89D\x02\xd34\
\t\x85B8\x8e\x83\xa2(\xccf3\xc6\xe31\xcd\xe6\x15\x9d\x8eM\xa3q\xca\xee\xee;<\
oD$2#\x18\x0e\x87\x89F\xa3\xd8\xb6\xcd\xf9\xf99\xe9t\x9ad2\x89\xae\xeb\x1c\
\x1e\x1e2\x18\x0c\x90R\x92L&q\x9c\x1b\\\xf7\x8e\xcd\xcd,\x0f\x0f\x16ww_\x11\
\xae\xeb\xfa\xfb\xfb\xfb\x00\xec\xed\xed\xd1j\xb5h\xb7\xdb\x04\x02\x01\xe6\
\xf3\',\xeb3\x99L\x86x<\x8e\xebNx|\xf4\x19\x0e\xbb\x08\xb1\xa0P( D`\r\xe7\
\xb6\xef_^^rrr\x82\xe7y\xe8\xba\x8ea\x184\x1a\x9f8;\xfb\x88\xa2\xa8h\xda\x1b\
\xe6s\x0fUU\xd8\xda\xda\xa2R\xa9P.\x97\xbf\x03\x00f\xd3\x89/\xa5\xa4\xdb\xed\
R\xaf\xd7\xb9\xb8\xb8 \x95z\xcb\xe9i\x9d\xe5r\xca\xc6\xc6\x06\xc5b\x91J\xa5B\
.\x97c}}\x9dX\\\x13\xe2e\x94\x17Os\xdf\xf3<\xfa\xfd>\x96e\xd1\xe9t0\x0c\x83b\
\xb1H*\x95B\xd34"J\xf4g\x16~\x01<\xd7\x83\'\xfd\xe9t\xca\x8fG\x07Ca\xf1r\xe7\
\x8f\x80\xbf\xd1\xff/\xd37un\xce\x10\xf4\xd0\xf0\x9c\x00\x00\x00\x00IEND\xae\
B`\x82\xd4\xbf\x04\x9e'
)

cdrom = (
'x\xda\x01\x10\x03\xef\xfc\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\
\x10\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\
\x08\x08\x08\x08|\x08d\x88\x00\x00\x02\xc7IDAT8\x8du\x93MO\x1bg\x10\xc7\x7f\
\xbb^\x9b]w\xa9\xd68\xb6A\xec*\n\xb2\x12\x05\xec\xc8)\x12\x88J\xa5-\xdf\xa5\
\x07\xbeH{\xa8\xd4O\xc0\xad\xb7H\xb9\xc0\x85&Bq\x05\xbdT\r/\x15\xb6Q\xec\xa6\
\xc5 avmcg\x17?[\xbf\xf0\xf4`\xa5i@\x1di\x0e3\x9a\xf9i\xfe3\x1aEQ#\xdc\xb6\
\x83\xfd\xd7\xb2\xdb\r0M\x0b\xcfk\x92N\x7fJ.\x97cB7\x94\xdb\xb5\xda\xed\xc4\
\xcb\x17?I\xdb\xb6\xc9f\x93\x08\x01\xe94\xd4\xebg<{\xf6\x9c\xceU[Z\x89\xa9\
\x8f \xea\x7f\x83_\xf6v\xe5\xca\xca\n\xc9d\x12)!\x0cA\x08\x98\x9ctH$>ckk\x0b\
\xff]W\xfe/\xa0\xdd\xbe\x01\xe2\x00\xc4{pO\x80\xa1\xc1h\x04\x0f\x1e<\xe4\xe4\
\xe4\x0fj\xb5\x1a\xc3A_\xde\x014=WJ\x99\xa2R\x81\x8b3\xf8\xad\xfc\x9a\xef~\
\xfc\x9e\xa9)\x88D@\xd3 \x163)\x95J\x08!\xee\xee\xc0\xf3<\xee\xdfw\xd0\xf5q\
\x83\xd7\xf6\xf1\xdf]\xa0i\xd0\xef\x8f\xfd\xea\xaaK\xa3\x01\xc3\xe1\xf0\xae\
\x84\xb9\xb99./\xdf\x8cam\xf82\xf7\x15\xdf\xae\xff@,\x06\xf9<8\x0e\xb8\xee_\
\x98\xa6I$\x12\xf9x\x82\x9b\xd1P\x02\xf4zm\x84\x18k>\xd7@\xd7a\x16\x88\xc5`w\
\xf79ax\x8dm\xdbh\xda\x87\xe3)\xadVK\x06A\xc0h4\xa2\xd5j\xf1\xf6m\x9f\'O>G\
\xd7\xc10`r\x12NN\xf6i4\x1a\xb8\xae\x8bm\xdb\xa4R)r\xb9\x1c\xd1\xd8\x84\xa2\
\xec\xec\xec\xc8l6K"\x91@UU<\xcf\xa3R\xf9\x13\xcb\xbaG\x18\x06\x9c\x9e\xbe\
\xc1\xb6g\x99\x9d\x9d\xa5\xdf\xef\xe3y\x1e\xaf^\xfd\x8c\xae\xc7Y_\xff\x06e{{\
[\xc6\xe3q\x86\xc3!\x96e\x91\xc9d\x88F\xa3\x08!8??g0\x18\x10\x8dF9:\xfa\x9dN\
\xe7\x1aU\x8d\x83\x19px\xf4+_\x14\xbeFk6\x9b\xcc\xcc\xcc\x90J\xa5\xf0}\x9fR\
\xa9\x84\xa6i\xf4z=\xaa\xd5*a\x18\xd2\xed\n,k\x8a\xe5\xe5e\x1cg\x06!\xae9\
\xab\\\xd0h4P:\x9d\x8e\xdc\xd8\xd8`0\x18\xb0\xb6\xb6\x86i\x9a\x04A\xc0\xe6\
\xe6&\x96\x95 \x95J\xf3\xe8\xd1CL\xf3\x13\x0e\x0eJ\x9c\x9e\x9eq|\\F\x88\x16\
\xab\xab\xab(\x8a\x1a\xe1\xb2q!\xcb\xe52\xc5b\x91 \x08\x98\x9e\x9e\xa6V\xab\
\xf1\xf4\xe92\xaaz\x83\xeb\xfa\x1c\x1f\x1f\xd1\xedz\x18\xc6\x04\x8e\xe3P(\
\x14XZZ\x1a\x03\x00\xfe\x0e\x85\xf4}\x9fz\xbd\xce\xde\xde\x1e\xc5b\x91\xc7\
\x8f\x0b\x1c\x1e\xee3\x1a\x85$\x93I\x16\x16\x16X\\\\d~~\x1e\xcb\xb20\x0c\xe3\
\x03\xe0\xbd\r\x07}\x19\x04\x01\xae\xebR.\x97\xa9V\xabd2\x19\xf2\xf9<\x8e\
\xe3`\x9a&\xba\x11\xff\xf7#\xff\x01\x15??\x04\x1c[\x91D\x00\x00\x00\x00IEND\
\xaeB`\x82\xf7@gb'
)

dir = (
'x\xda\x01\xdc\x01#\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\x93IDAT8\x8d\xa5\x92\xbfkSQ\x14\xc7?\xf7%\
\xf0\x088\xb4\xb6\xde\xd0?\xc0"\xdd\x8a%\x83cPp\xcbb)\xf8\x07ts\x11\x1cup\
\xedPpP\xda\xd9E)\xf8\xe8n0\x0e\x05i\'\xb3\x84B)d\x11\x9e\xfa\xa64y\xf9q\xce\
\xe9\xf0\x92\x10\xd2\xfa,\xf4\x0b\x07\xee\xe5\xde\xfb\xb9\xdfs\xf8:\x17\x14\
\xb8\x8d\x82[\xbd\x06\x8a\x93\xc5\xf1\x87u33T\x15Ue\xf1\xf1\'\xbc\xf7,-\xdfs\
\xb9\x04\x17\x148\xd9\xdb\xb0y}\xdf]\xb3F\xa3aI\x92\x98\x0b\n\xfc\xb3N\xf66\
\xec\xe1\xf61\xb0?\x87\xde\xa6\xbe\xb3\x8a\x88 "\x993SD\x84\x07\xcf\xbfQ.\
\x97\xb9\xbb\xb4\xec\x8af\x06\xe9;\x10\x99\x03\xecR}uz\xc5\xf1\x97\xd7e\xa2(\
\xa2V\xabe\x1d\xfcx\xbfn\x95\xcdg0\x1cqtppm\x9b\xc3\xe1\x90\xd1h4-\xc3\x00\
\xb8\xbf\xf5\x95\xa2\xaa\xc2E\x97\xa3\xc3C\x1e\xbd\xf8\x99?r\x80_oa\xe5\r\
\x1f_\xde!\x8a\xa21\xa0\xd3!\x0c\xc3k\xe6pU\xfd\xf3s\xc2\x95}D\x84v\xbbMQD M\
1\xcbl\x91\xa6\xb9\x80^\xafG\x08\xa8*f6v\x00\x0c\x06\x83\xecF\xa7\xf3_\xc0\
\xc2\x18\x00\x8c\x1d\xcc\x02.\xba\xb9\x80n7;WUp3\x80~\xbf\x7fc\x07\x00f\x86\
\x0b\x1c\xae\xd5j\xd9\xd9\xe7\'\xd3\x90\xa8\xea4<\xb3\xfbI\xc4\'\xbdG\xed\
\xa7T*\x15\\\x92$\xd6l6\xa9\xd7\xeb\xc4q\x9c\xfb\xfb\xac\xbc\xf7T\xabU\x9c\
\x0b\n\xfc\xfd\xf3\xdb\xe28\x9e\xda\xbb\x89J\xa5\x12\xde{.\x01\x95y\xfd\x8dS\
\x0f\x01\x13\x00\x00\x00\x00IEND\xaeB`\x82\xc6#\xd7\xe1')

file = (
'x\xda\x01\xd7\x01(\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01\x8eIDAT8\x8d\xa5\x90;kTQ\x14\x85\xbfs\xc7\
\x9f`-\x82DkA\xc5&\x8d\x88EJ\xb5\x12K\x83 \x06\x82\x9d\x84`#\xda)>\x1a1\x95\
\x19\x14\x91h\xb4\xd0"&\x0c\x82\x8d\xd8\nF\xf1?$\x99\x11\xe6\x9c\xfd\xb2\xc8\
\x9d\x91\xe1N&\x88\xbb\xd9\xa7X\xeb;k\xed\x94\xaa\x16\xff3\x07\x06\x8f\xb7\
\x9do1N\xa0\xaa\x88\x08R\nS\x87\x0er\xfa\xf8\xd14"HU\x8bT\xb5x\xf7\xe9{\xec5\
\x1e\x11K\xaf:qj\xe6J\xb4\xdfl\xc4\xc0\x93\xaa\x16\xd5\x00dfc#F\x80Y\x90s\
\xe6\xf6\xc2<\x0f\x9f>gyem\x98vXAU\x1bf\x0fp\x0b\xcc\x03wg\xf1\xce\x03RJ<Zz\
\xd1\xbc\x81\x8a\x00p\xe2\xfa&\xae}\xac\xf4\xb0\xdc\xc5\xf36_\x96/r\xf9\xfc\
\xd9\xa1\xa9\xfd\xf2\xf5\x18@\x9d\xe0\xeb\xe3c\x98\x83\xb9c\x0e\xaa\x81h\xa0\
\x06\x17f\xce\xb0\xf2~\x83\x88h\x02D\x85\x88\xe0\xe4\xdc\x0f\\\xfbx\x9d@\xf3\
\x16\x9ew\xb0\xf2\x9b\x8f\xcf\xae5j\x8eT0\x87\xcf\xf7\xa6P\x07\xb5\xc0,\x10\
\x0bT\x1d\xb1@\x8aO\x00\xa8"\x16L\xdf\xf89\xbc\x81\xe7.\x96\xb7\xd1\xbc\x83\
\x97\x1e\xabO\xae\xee\r03\xcc\x82\xb5\xbbGP\xa3\xfe\x15\x8a9\xaa\x81\x9aSdB\
\x02\x80\xa2\xc1\xb9\x9b\xbf\x08\xe9\xa3R\'\xe8oa\xa5\x8b\x95\x1e\xed\xfb\
\xb3\x93\x01\xa2\xce\xea\xad\xc3\x88\xf2\xb7\xbb\x82X}\x03\xdd/\x81\x80\x18\
\x88\xee\x8aw!1\xb2\xf7\xa9\xe0|X\xef4D\x93f\x04piv\xee\x9f\xcc\x00\x7f\x00{\
G<T[\xfe\x1c\xc8\x00\x00\x00\x00IEND\xaeB`\x82\x17+\xd2\x15'
)

db3 = (
'x\xda\xeb\x0c\xf0s\xe7\xe5\x92\xe2b``\xe0\xf5\xf4p\t\x02\xd2\x02 \xcc\xc1\
\x06$\xe5?\xffO\x04R,\xc5N\x9e!\x1c@P\xc3\x91\xd2\x01\xe4o\xf3tq\x0c\xb1\xe8\
=;\xf9l\xeeQ\x03\x9ef{K\xd9#\xb3#\x16Wp\xc7[\xce\xda\xe3it\xa2\xfa\xd5R\xc3\
\xb8\x06\xa6\xbf\'\x17\xaa\x1e\x98\xec\xd1\xc9\xa4\xb2yS!\xfb\xd3i\xd2\x07Nm\
\xe3X\xb2\x84\xc5\xeeid\xf5\xd7\x19S{k\xec\x82\x84\x18\xde\xf19d\xbf5\xca8\
\xc7\xe9\xe0\xc2\xa0\xc0\xa9=\x9b1\xc5\xe5\x89\xbf\xa0 \xe7\x93\xd9*w\xe6\
\xdeW\x99\xe0"\x99\x1c\xd3"\xd8\xb8;9M|\xd7o\xa3\xf9\x12\xc9\t\xf2\xda\x17\
\xa2\xa2\x83\xf7\x1dZYZn\xe6\xf9jor\xda\x9b\xdf~kK;\xb3\xd6v\x1f/\\\xd3R\xec\
W\xf5\xfe\x7fJ\xeb\xed\xf2\x84\xf5\xfb\'\x9cw\xafS?X\xe9\xc0\xf0\xed\xf9\xf9\
\x92\xdd\x07\xad\xfb\x9f,?[-\xe1\xb1\x0b\xe8\x03\x06OW?\x97uN\tM\x00\xbf\x89\
e&'
)

xml = (
'x\xda\x01n\x01\x91\xfe\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\
\x00\x00\x00\x10\x08\x06\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\
\x08\x08\x08|\x08d\x88\x00\x00\x01%IDAT8\x8d\xd5\x93=R\x02A\x10\x85\xbf^\x8c\
\x87cx\x00\x90\x982\xf5\x00(\x07\x80\x13\x18Pj\t\xa5Nb@\x0c\x07X\xe5\x00\xa4\
\x16\xf1Z\x1c\x80\x0b\x183{\x81g\xb0\xee\xb8\x8b?HhWuu\xcd\xf4\x9b\x9e\xd7=o\
\xcc{/\x0e\xb0\xab\xeb\x1b\xabmx\xef\xf5W[\xadV\xf2\xde\xcb\x92\x06\xa5\'\
\xfbn\xcc\xf3\xbc\x16G\xa3\x11\x0f\xf7w\x91\xf5\xd1.\xd89\xf7c\xecv\xbbU\xb8\
,iXR=\xbc\xdc(\x82m2\xa19\x9d\xd2\x1e\x12\x8b\xa4\xd9\x96\x10\x02Rel\xe5\x0c\
\xd2l+\x98I\x92\x18\x8f%I-I\xad\x99\x04\xc5\x1afJ\xb3\xadB\x08\xfa`P\xcc\xe0\
\xe95\xd0\xef,\x90\x06\xb1\xd76\xc0\x1a\xc0"Ci@\xbf\xb3`\xb9\xf9d\x90\x00\\\
\x9c4I\xb3\x1efs\x9cs\xb4\xdena\x0e\xac\x8d\xf5\xb0\x00;\xe70\x9b\x93f=\xce\
\x8e+/Y\xb6\x10B\x88\xf4\n\xba\xe3\xe8\xdf\xe7\x8b\x16j:(\x93\x7f\x89e\x81\
\xf8\x8cy\x9esz\xf9\xbcO\x16\xbc<\x9e\xe3\x9c\xfb\xda\xc2\xffePS\xe2o2\xde\
\x8d\xa5\x19p\xd0w\xae\x9a%\r{\x07\xb0d\x9bs<\xe5\xe3\x12\x00\x00\x00\x00IEN\
D\xaeB`\x82\x97\x01\x9aF'
)

table = (
"x\xda\xeb\x0c\xf0s\xe7\xe5\x92\xe2b``\xe0\xf5\xf4p\t\x02\xd2\x02 \xcc\xc1\
\x06$\xe5?\xffO\x04R,\xc5N\x9e!\x1c@P\xc3\x91\xd2\x01\xe47x\xba8\x86X\xf4\
\xbe\x9dd\xc8w\xd8\x80\xc3U~%\xff\xcd\xd5\x15E5\xd6}\x07\xce\xa8J\xbc\x0cP\r\
]\xe3\xb1\xf6\xf2R\xe7\x99?:\xfe\x7fm\x11\x88\xcc\xab\tf\xca\x8e3\x9d}W\xf3\
\xfc\xb6\x89\x9cJ\xc7[\x0b\xcb\xae\xc8\x86\xa6\xde\x0cd\xfb|3\xe7\xca\xbb}\
\x82_*\xeb\x14\x9446L\xb2\xacg\x13d|\xe5\xc6\xb0w\xbfFTv\xfeW9\x96'\xa1MQ\
\xffY\xec]\x14\xd2\xeb\xff$\x8a\x1a;\x87\x16\x96+%Y=`\x14\xe8\x896^w\xec\xea\
;\xa0[\x18<]\xfd\\\xd69%4\x01\x004\x1fHY"
)

index = (
'x\xda\xeb\x0c\xf0s\xe7\xe5\x92\xe2b``\xe0\xf5\xf4p\t\x02\xd2\x02 \xcc\xc1\
\x06$\xe5?\xffO\x04R,\xc5N\x9e!\x1c@P\xc3\x91\xd2\x01\xe47z\xba8\x86X\xf4\
\xbe\x9d\xb4Q\xf0\xb0\x01\x87\xab|_\xdb\x19\xadU)\xb9\xc79VE6\t\x89[\xdeN\
\xbf\xd9\xfa\xd4D\xd3\xe9\xe8\xf3\x1fw\x9a\x9dS\xaa>\xfar\x9c\xf9x\xe5\xce\
\xad\xd9_<\xb9\x05\x15\x8ar^d\xbb\xc7\x06\x86.\xe3]j\x1c\x7f\xb5*7M\xdbQ\xf6\
9\xfb\x9d\xb9,\xc6\x82\x8f\xd3\x05\x04\x15\xd6\xb3\xcc\x99\xb6\xb6t\xb9\xd6\
\xd29_\xd2\x98\x1fi:\x9e\xabo\x92W\x9a\xb0\xda~\x9f\xe4\xca\xadm\xab\x92\xd2\
v\xfc\xfe\xd0\xc0\xfa\xffI\xb8q\xcd\xe3\xf7|@\xc70x\xba\xfa\xb9\xacsJh\x02\
\x00\xfe\xe3L6'
)

#-------------------------------------------------------------------------------
# Eof
#-------------------------------------------------------------------------------