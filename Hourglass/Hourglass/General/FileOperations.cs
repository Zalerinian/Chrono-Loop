using System;
using Microsoft.DirectX;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Xml;
using System.Diagnostics;
using System.Collections.Generic;

namespace Hourglass
{

	public static class FileIO
	{
		public static readonly uint WriterVersion = 1;
		public static readonly float RADIANS_TO_DEGREES = ((180.0f / 3.14f));
		public static readonly float DEGREES_TO_RADIANS = (1 / 180.0f * 3.14f);
		private static Stack<BaseObject> ObjectStack = new Stack<BaseObject>();

		enum ObjectOperation { OP_NONE, OP_PUSH, OP_POP };

		public static bool LoadSettings()
		{
			if (!File.Exists("settings.exs"))
			{
				return false;
			}
			XmlReaderSettings s = new XmlReaderSettings();
			s.DtdProcessing = DtdProcessing.Parse;
			XmlReader reader = XmlReader.Create("settings.exs", s);
			//reader.MoveToContent();
			while (reader.Read())
			{
				switch (reader.NodeType)
				{
					case XmlNodeType.Element:
						switch (reader.Name)
						{
							case "SettingsVersion":
								// Ignore this for now.
								break;
							case "ProjectPath":
								reader.Read();
								Settings.ProjectPath = reader.Value;
								break;
							case "BackgroundColor":
								reader.Read();
								Settings.BackgroundColor = Color.FromArgb(int.Parse(reader.Value));
								break;
							default:
								Debug.Print("Unexpected element type: " + reader.Name);
								break;
						}
						break;
				}
			}
			reader.Close();
			return true;
		}

		public static bool SaveSettings()
		{
			XmlWriterSettings s = new XmlWriterSettings();
			s.Indent = true;
			s.OmitXmlDeclaration = true;
			XmlWriter writer = XmlWriter.Create("settings.exs", s);

			writer.WriteStartElement("Settings");
			writer.WriteElementString("SettingsVersion", Settings.SettingsVersion.ToString());
			writer.WriteElementString("ProjectPath", Settings.ProjectPath);
			writer.WriteElementString("BackgroundColor", Settings.BackgroundColor.ToArgb().ToString());
			writer.WriteEndElement();

			writer.Close();
			return true;
		}


		public static void saveLevel(string file, TreeView tree)
		{
			// Binary file writing.
			FileStream fs = new FileStream(file, FileMode.Create);
			using (BinaryWriter writer = new BinaryWriter(fs))
			{
				Int32 SettingsOffset = 0, ObjectsOffset = 0;
				// Header
				//  * File Writer Version
				//  * Section offsets
				writer.Write(WriterVersion);
				// Section offsets are initially 0, but we need to reserve the space here, so write 0s.
				// There is no offset for editor settings, as only the editor needs to know about these,
				// and it is located immediately after the header.
				writer.Write(0);
				writer.Write(0);

				// Editor settings
				// TODO: Write editor data

				// Level Settings
				SettingsOffset = (Int32)writer.BaseStream.Position;
				writer.Write(Settings.StartPos.X);
				writer.Write(Settings.StartPos.Y);
				writer.Write(Settings.StartPos.Z);

				writer.Write(Settings.StartRot.X * DEGREES_TO_RADIANS);
				writer.Write(Settings.StartRot.Y * DEGREES_TO_RADIANS);
				writer.Write(Settings.StartRot.Z * DEGREES_TO_RADIANS);


				// Level Objects
				writer.Write((byte)0x4B);
				writer.Write((byte)0x4B);
				writer.Write((byte)0x4B);
				writer.Write((byte)0x4B);
				ObjectsOffset = (Int32)writer.BaseStream.Position;
				writer.Write(0); // Data to be written later
				int ObjectCount = 0;
				for (int i = 0; i < tree.Nodes.Count; ++i)
				{
					WriteObject(writer, tree.Nodes[i], ref ObjectCount);
				}

				writer.Seek(sizeof(Int32), SeekOrigin.Begin);
				writer.Write(SettingsOffset);
				writer.Write(ObjectsOffset);
				writer.Seek(ObjectsOffset, SeekOrigin.Begin);
				writer.Write(ObjectCount);
			}

			#region XML Writing
			//XmlWriterSettings settings = new XmlWriterSettings();
			//settings.Indent = true;
			//settings.OmitXmlDeclaration = true;
			//using (XmlWriter writer = XmlWriter.Create(file, settings)) {
			//    writer.WriteStartDocument();
			//    writer.WriteStartElement("Level");
			//    //*** Level Settings ***//
			//    writer.WriteStartElement("Settings");
			//    writer.WriteElementString("StartPos", mStartPos.X + "," + mStartPos.Y + "," + mStartPos.Z);
			//    writer.WriteElementString("StartRot", (mStartRot.X * DEGREES_TO_RADIANS) + "," + (mStartRot.Y * DEGREES_TO_RADIANS) + "," + (mStartRot.Z * DEGREES_TO_RADIANS));
			//    writer.WriteEndElement();
			//    //*** END Level Settings ***//
			//    foreach (ToolObject tObj in higharchy) {
			//        writer.WriteStartElement("Object");
			//        writer.WriteElementString("Name", tObj.Name);
			//        if (tObj.MeshFile != null)
			//            writer.WriteElementString("Mesh", tObj.MeshFile.Split('\\').Last());
			//        if (tObj.TextureFile != null)
			//            writer.WriteElementString("Texture", tObj.TextureFile == null ? "" : tObj.TextureFile.Split('\\').Last());
			//        writer.WriteElementString("Position", tObj.Position.X + "," + tObj.Position.Y + "," + tObj.Position.Z);
			//        writer.WriteElementString("Rotation", tObj.Rotation.X + "," + tObj.Rotation.Y + "," + tObj.Rotation.Z);
			//        writer.WriteElementString("Scale", tObj.Scale.X + "," + tObj.Scale.Y + "," + tObj.Scale.Z);
			//        writer.WriteStartElement("Components");
			//        foreach (string str in tObj.Components) {
			//            switch (str) {
			//                case "Box Snap":
			//                    writer.WriteElementString("BoxSnapToController", "Enabled");
			//                    break;
			//                case "Button Press":
			//                    writer.WriteElementString("ButtonPress", "Enabled");
			//                    break;
			//                case "AABB to AABB":
			//                    writer.WriteElementString("AABBtoAABB", "Enabled");
			//                    break;
			//                case "AABB to Sphere":
			//                    writer.WriteElementString("AABBtoSphere", "Enabled");
			//                    break;
			//                case "Elastic Plane":
			//                    writer.WriteElementString("ElasticPlane", "Enabled");
			//                    break;
			//                case "Sphere to Sphere":
			//                    writer.WriteElementString("SpheretoSphere", "Enabled");
			//                    break;
			//                case "Enter Level":
			//                    writer.WriteElementString("EnterLevel", "Enabled");
			//                    break;
			//                case "Gesture":
			//                    writer.WriteElementString("Gesture", "Enabled");
			//                    break;
			//                case "Headset Follow":
			//                    writer.WriteElementString("HeadsetFollow", "Enabled");
			//                    break;
			//                case "Main Menu":
			//                    writer.WriteElementString("MainMenu", "Enabled");
			//                    break;
			//                case "Teleport":
			//                    writer.WriteElementString("Teleport", "Enabled");
			//                    break;
			//                default:
			//                    break;
			//            }
			//        }
			//        if (tObj.Collider != null) {
			//            writer.WriteStartElement("Collider");
			//            writer.WriteElementString("Type", tObj.ColliderType);
			//            writer.WriteElementString("Trigger", tObj.Collider.IsSolid ? "False" : "True");
			//            writer.WriteElementString("Position", tObj.Collider.Position.X + "," + tObj.Collider.Position.Y + "," + tObj.Collider.Position.Z);
			//            if (tObj.ColliderType == "Sphere")
			//                writer.WriteElementString("Radius", tObj.Collider.Scale.X.ToString());
			//            else {
			//                writer.WriteElementString("Rotation", tObj.Collider.Rotation.X + "," + tObj.Collider.Rotation.Y + "," + tObj.Collider.Rotation.Z);
			//                writer.WriteElementString("Scale", tObj.Collider.Scale.X + "," + tObj.Collider.Scale.Y + "," + tObj.Collider.Scale.Z);
			//            }
			//            writer.WriteElementString("Move", tObj.Collider.CanMove ? "True" : "False");
			//            if (tObj.ColliderType == "Sphere" || tObj.ColliderType == "OBB") {
			//                writer.WriteElementString("Gravity", tObj.Collider.Gravity.X + "," + tObj.Collider.Gravity.Y + "," + tObj.Collider.Gravity.Z);
			//                writer.WriteElementString("Mass", tObj.Collider.Mass.ToString());
			//                writer.WriteElementString("Elasticity", tObj.Collider.Elasticity.ToString());
			//                writer.WriteElementString("StaticFriction", tObj.Collider.StaticF.ToString());
			//                writer.WriteElementString("KeneticFriction", tObj.Collider.KeneticF.ToString());
			//                writer.WriteElementString("Drag", tObj.Collider.Drag.ToString());
			//            } else if (tObj.ColliderType == "Plane") {
			//                writer.WriteElementString("Normal", tObj.Collider.Gravity.X + "," + tObj.Collider.Gravity.Y + "," + tObj.Collider.Gravity.Z);
			//                writer.WriteElementString("Mass", tObj.Collider.Mass.ToString());
			//                writer.WriteElementString("Elasticity", tObj.Collider.Elasticity.ToString());
			//                writer.WriteElementString("StaticFriction", tObj.Collider.StaticF.ToString());
			//                writer.WriteElementString("KeneticFriction", tObj.Collider.KeneticF.ToString());
			//                writer.WriteElementString("Drag", tObj.Collider.Drag.ToString());
			//            } else if (tObj.ColliderType == "Button") {
			//                writer.WriteElementString("PushNormal", tObj.Collider.Gravity.X + "," + tObj.Collider.Gravity.Y + "," + tObj.Collider.Gravity.Z);
			//                writer.WriteElementString("Mass", tObj.Collider.Mass.ToString());
			//                writer.WriteElementString("NormalForce", tObj.Collider.StaticF.ToString());
			//            }
			//            writer.WriteEndElement();
			//        }
			//        writer.WriteEndElement();
			//        writer.WriteEndElement();
			//    }
			//    writer.WriteEndElement();
			//    writer.WriteEndDocument();
			//    writer.Close();
			//    FileChanged = false;
			//}
			#endregion
		}

		private static void WriteObject(BinaryWriter w, TreeNode n, ref int ObjectCount)
		{
			// Format:
			// Number of components
			// all the components
			// Object Operation (Push, Pop, None)
			// repeat

			++ObjectCount;
			BaseObject b = (BaseObject)n.Tag;
			List<Component> comps = b.GetComponents();
			w.Write(comps.Count);
			for (int i = 0; i < comps.Count; ++i)
			{
				comps[i].WriteData(w);
			}
			if (n.Nodes.Count > 0)
			{
				// Write the push command, and the recurse through our children.
				w.Write((byte)ObjectOperation.OP_PUSH);

				for(int i = 0; i < n.Nodes.Count; ++i)
				{
					WriteObject(w, n.Nodes[i], ref ObjectCount);
				}
				// Now that children are done writing, overwrite the previous "none" command by the last object with no children
				// with a pop command so that we follow "object, op, object, op" model.
				w.Seek(-1, SeekOrigin.Current);
				w.Write((byte)ObjectOperation.OP_POP);
			}
			else
			{
				// No children, write in the no-op command.
				w.Write((byte)ObjectOperation.OP_NONE);
			}
		}

		public static void openLevel(string _file, TreeView _tree)
		{
			FileStream fs = null;
			try
			{
				fs = new FileStream(_file, FileMode.Open);
			}
			catch (FileNotFoundException)
			{
				MessageBox.Show("The specified file (" + _file + ") could not be found. Make sure it is in the right folder, and hasn't been deleted.", "Error opening file", MessageBoxButtons.OK);
				return;
			}
			using (BinaryReader reader = new BinaryReader(fs))
			{
				int vers = reader.ReadInt32();
				if(WriterVersion != vers)
				{
					MessageBox.Show("The selected file was written in a different version of Hourglass. We'll see how this goes...", "Tread Carefully...", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
				}
				int settingsOffset = 0, objectsOffset = 0;
				settingsOffset = reader.ReadInt32();
				objectsOffset = reader.ReadInt32();
				Vector3 startPos = new Vector3(), startRot = new Vector3();

				startPos.X = (float)reader.ReadInt32();
				startPos.Y = (float)reader.ReadInt32();
				startPos.Z = (float)reader.ReadInt32();

				startRot.X = (float)reader.ReadInt32();
				startRot.Y = (float)reader.ReadInt32();
				startRot.Z = (float)reader.ReadInt32();

				Settings.StartPos = startPos;
				Settings.StartRot = startRot;

				// There are 4 padding bytes, for the giggles.
				reader.ReadBytes(4);
				int ObjectCount = reader.ReadInt32();

				for(int i = 0; i < ObjectCount; ++i)
				{
					ReadObject(reader, _tree);
				}
			}
		}

        public static void ReadXMLFile(string _file, TreeView _tree) {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            OpenFileDialog openFileDialog2 = new OpenFileDialog();

            openFileDialog1.InitialDirectory = Application.StartupPath;
            openFileDialog1.Filter = "Compatible Files (*.xml, *.obj)|*.xml;*.obj";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK) {
                try {
                    XmlReaderSettings settings = new XmlReaderSettings();
                    settings.DtdProcessing = DtdProcessing.Parse;
                    XmlReader reader = XmlReader.Create(openFileDialog1.FileName, settings);
                    reader.MoveToContent();
                    string element = string.Empty, mesh = string.Empty, texutre = string.Empty, name = string.Empty;
                    TreeNode node = new TreeNode();
                    TexturedMeshComponent tmc = null;
                    ColliderComponent col = null;
                    _tree.Nodes.Add(node);
                    BaseObject addition = new BaseObject(node);
                    bool collider = false;
                    while (reader.Read()) {
                        string[] parts = { };
                        Vector3 point = new Vector3();
                        switch (reader.NodeType) {
                            case XmlNodeType.Element:
                                switch (reader.Name) {
                                    case "Object":
                                        node = new TreeNode();
                                        _tree.Nodes.Add(node);
                                        addition = new BaseObject(node);
                                        mesh = texutre = string.Empty;
                                        collider = false;
                                        break;
                                    case "Collider":
                                        collider = true;
                                        break;
                                    default:
                                        element = reader.Name;
                                        break;
                                }
                                break;
                            case XmlNodeType.Text:
                                switch (element) {
                                    case "StartPos":
                                        Vector3 pos = new Vector3();
                                        parts = reader.Value.Split(',');
                                        pos.X = float.Parse(parts[0]);
                                        pos.Y = float.Parse(parts[1]);
                                        pos.Z = float.Parse(parts[2]);
                                        Settings.StartPos = pos;
                                        break;
                                    case "StartRot":
                                        Vector3 rot = new Vector3();
                                        parts = reader.Value.Split(',');
                                        rot.X = float.Parse(parts[0]) * RADIANS_TO_DEGREES;
                                        rot.Y = float.Parse(parts[1]) * RADIANS_TO_DEGREES;
                                        rot.Z = float.Parse(parts[2]) * RADIANS_TO_DEGREES;
                                        Settings.StartRot = rot;
                                        break;
                                    case "Name":
                                        name = reader.Value;
                                        break;
                                    case "Mesh":
                                        tmc = new TexturedMeshComponent();
                                        tmc.SelectMesh(tmc.CheckForMesh(reader.Value));
                                        addition.AddComponent(tmc);
                                        break;
                                    case "Texture":
                                        if (tmc == null) {
                                            tmc = new TexturedMeshComponent();
                                            addition.AddComponent(tmc);
                                        }
                                        tmc.SelectTexture(tmc.CheckForTexture(reader.Value));
                                        break;
                                    case "Position":
                                        parts = reader.Value.Split(',');
                                        point.X = float.Parse(parts[0]);
                                        point.Y = float.Parse(parts[1]);
                                        point.Z = float.Parse(parts[2]);
                                        if (collider) {
                                            col.Shape.Position = point;
                                        } else {
                                            ((TransformComponent)addition.GetComponents()[0]).SetPosition(point);
                                        }
                                        break;
                                    case "Rotation":
                                        parts = reader.Value.Split(',');
                                        point.X = float.Parse(parts[0]);
                                        point.Y = float.Parse(parts[1]);
                                        point.Z = float.Parse(parts[2]);
                                        if (collider) {
                                            col.Shape.Rotation = point;
                                        } else {
                                            ((TransformComponent)addition.GetComponents()[0]).SetRotation(point);
                                        }
                                        break;
                                    case "Scale":
                                        parts = reader.Value.Split(',');
                                        point.X = float.Parse(parts[0]);
                                        point.Y = float.Parse(parts[1]);
                                        point.Z = float.Parse(parts[2]);
                                        if (collider) {
                                            col.Shape.Scale = point;
                                        } else {
                                            ((TransformComponent)addition.GetComponents()[0]).SetScale(point);
                                        }
                                        break;
                                    case "Type":
                                        if (reader.Value == "Sphere") {
                                            col = new SphereCollider();
                                            ((ColoredShape)col.Shape).Load("Assets\\Sphere.obj", Color.Red);
                                        }
                                        else if (reader.Value == "OBB" || reader.Value == "Button") {

                                            col = new BoxCollider();
                                            ((ColoredShape)col.Shape).Load("Assets\\Cube.obj", Color.Red);
                                        }
                                        else {
                                            col = new PlaneCollider();
                                            ((ColoredShape)col.Shape).Load("Assets\\Plane.obj", Color.Red);
                                        }
                                        break;
                                    case "Trigger":
                                        if (reader.Value == "True")
                                            addition.Collider.IsSolid = false;
                                        else
                                            addition.Collider.IsSolid = true;
                                        break;
                                    case "Radius":
                                        float radius = float.Parse(reader.Value);
                                        addition.Collider.SetScale(new Vector3(radius, radius, radius));
                                        break;
                                    case "PushNormal":
                                    case "Normal":
                                    case "Gravity":
                                        parts = reader.Value.Split(',');
                                        point.X = float.Parse(parts[0]);
                                        point.Y = float.Parse(parts[1]);
                                        point.Z = float.Parse(parts[2]);
                                        addition.Collider.Gravity = point;
                                        break;
                                    case "Move":
                                        addition.Collider.CanMove = reader.Value == "True";
                                        break;
                                    case "Mass":
                                        addition.Collider.Mass = float.Parse(reader.Value);
                                        break;
                                    case "Elasticity":
                                        addition.Collider.Elasticity = float.Parse(reader.Value);
                                        break;
                                    case "NormalForce":
                                    case "StaticFriction":
                                        addition.Collider.StaticF = float.Parse(reader.Value);
                                        break;
                                    case "KeneticFriction":
                                        addition.Collider.KeneticF = float.Parse(reader.Value);
                                        break;
                                    case "Drag":
                                        addition.Collider.Drag = float.Parse(reader.Value);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case XmlNodeType.EndElement:
                                break;
                            default:
                                break;
                        }
                    }
                    reader.Close();
                } catch (Exception ex) {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }
        }

        private static void ReadObject(BinaryReader r, TreeView tree)
		{
			// Format:
			// Number of components
			// all the components
			// Object Operation (Push, Pop, None)
			// repeat
			TreeNode n = new TreeNode();
			BaseObject b = new BaseObject(n);
			n.Tag = b;
			if(ObjectStack.Count > 0)
			{
				b.Parent = ObjectStack.Peek();
				b.Parent.Node.Nodes.Add(n);
			}
			else
			{
				tree.Nodes.Add(n);
			}


			int CompCount = r.ReadInt32();
			for(int i = 0; i < CompCount; ++i)
			{
				short compType = r.ReadInt16();
				Component com = null;
				switch(compType)
				{
					case (short)Component.ComponentType.Code:
						// TODO: Code components
						break;
					case (short)Component.ComponentType.Transform:
						b.GetComponents()[0].ReadData(r);
						n.Text = ((TransformComponent)b.GetComponents()[0]).Name;
						break;
					case (short)Component.ComponentType.BoxCollider:
						com = new BoxCollider();
						com.ReadData(r);
						b.AddComponent(com);
						break;
					case (short)Component.ComponentType.ButtonCollider:
						com = new ButtonCollider();
						com.ReadData(r);
						b.AddComponent(com);
						break;
					case (short)Component.ComponentType.PlaneCollider:
						com = new PlaneCollider();
						com.ReadData(r);
						b.AddComponent(com);
						break;
					case (short)Component.ComponentType.SphereCollider:
						com = new SphereCollider();
						com.ReadData(r);
						b.AddComponent(com);
						break;
					case (short)Component.ComponentType.ColoredMesh:
						com = new ColoredMeshComponent();
						com.ReadData(r);
						b.AddComponent(com);
						break;
					case (short)Component.ComponentType.TexturedMesh:
						com = new TexturedMeshComponent();
						com.ReadData(r);
						b.AddComponent(com);
						break;
                    case (short)Component.ComponentType.Audio:
                        com = new SoundComponent();
                        com.ReadData(r);
                        b.AddComponent(com);
                        break;
					default:
						Debug.Print("An unexpected component type has bee found. This may indicate corruption: " + compType);
						break;
				}
			}
			byte op = r.ReadByte();
			switch(op)
			{
				case (byte)ObjectOperation.OP_NONE:
					break;
				case (byte)ObjectOperation.OP_PUSH:
					ObjectStack.Push(b);
					break;
				case (byte)ObjectOperation.OP_POP:
					ObjectStack.Pop();
					break;
			}
		}

	}
} // LevelEditor Namespace