<patch-1.0 appVersion="1.0.12">
   <obj type="ksoloti/gills/encoder" uuid="8d8c7129-cce1-491f-98b4-6ecb6355cfee" name="encoder_1" x="14" y="14">
      <params/>
      <attribs>
         <spinner attributeName="min" value="0"/>
         <spinner attributeName="max" value="0"/>
      </attribs>
   </obj>
   <obj type="rbrt/control/click hold S" uuid="99efc8b8-7803-4f15-8ab2-8d0adace10c6" name="click_1" x="126" y="14">
      <params/>
      <attribs>
         <spinner attributeName="long" value="3000"/>
      </attribs>
   </obj>
   <patchobj type="patch/object" uuid="f63597e0-40c5-4cf3-8442-123b99498a23" name="loadSD_startPatch" x="266" y="14">
      <params/>
      <attribs/>
      <object id="patch/object" uuid="f63597e0-40c5-4cf3-8442-123b99498a23">
         <sDescription>Stops current patch, and loads a patch from sdcard, selected by index in the patch bank file.
If the index is out of range, the sdcard startup patch is loaded instead.
</sDescription>
         <author>Johannes Taelman</author>
         <license>BSD</license>
         <inlets>
            <bool32.rising name="trig" description="trigger"/>
         </inlets>
         <outlets/>
         <displays/>
         <params/>
         <attribs/>
         <includes/>
         <code.declaration><![CDATA[int ntrig;]]></code.declaration>
         <code.init><![CDATA[ntrig = 1;]]></code.init>
         <code.krate><![CDATA[if (inlet_trig && !ntrig) {
	ntrig = 1;
	codec_clearbuffer();
	LoadPatchStartSD();
}
else if (!inlet_trig) {
	ntrig = 0;
}]]></code.krate>
      </object>
   </patchobj>
   <nets>
      <net>
         <source obj="click_1" outlet="long"/>
         <dest obj="loadSD_startPatch" inlet="trig"/>
      </net>
      <net>
         <source obj="encoder_1" outlet="sw"/>
         <dest obj="click_1" inlet="gate"/>
      </net>
   </nets>
   <settings>
      <subpatchmode>no</subpatchmode>
   </settings>
   <notes><![CDATA[]]></notes>
   <windowPos>
      <x>240</x>
      <y>160</y>
      <width>494</width>
      <height>261</height>
   </windowPos>
</patch-1.0>