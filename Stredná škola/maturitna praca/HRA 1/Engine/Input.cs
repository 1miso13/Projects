using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace MojEngine
{
    public class Input
    {
        Engine Engine;
        public Input(Engine en)
        {
            Engine = en;
        }
        /// <summary>
        /// Centovanie myše
        /// </summary>
        public void CenterMouse()
        {
            Mouse.SetPosition(Engine.GraphicsDevice.PresentationParameters.BackBufferWidth / 2, Engine.GraphicsDevice.PresentationParameters.BackBufferHeight / 2);
            mysState = Mouse.GetState();
        }
        public void Update(){
            
                staryState = state;
                state = Keyboard.GetState();
                mysStaryState = mysState;
                mysState = Mouse.GetState();
            
            
        }
#region klavesnica
        KeyboardState state;
        KeyboardState staryState;

        public bool StisknutaKlavesa(Keys co)
        {
            return staryState.IsKeyUp(co) && state.IsKeyDown(co);
        }

        public bool DrzenaKlavesa(Keys co)
        {
            return staryState.IsKeyDown(co) && state.IsKeyDown(co);
        }

        public bool PustenaKlavesa(Keys co)
        {
            return staryState.IsKeyDown(co) && state.IsKeyUp(co);
        }
#endregion klavesnica
#region myš
        MouseState mysState;
        MouseState mysStaryState;

        public int DeltaX
        {
            get { return mysState.X - mysStaryState.X; }
        }
        public int DeltaY
        {
            get { return mysState.Y - mysStaryState.Y; }
        }
        public int DeltaKolecko
        {
            get
            {
                return mysState.ScrollWheelValue - mysStaryState.ScrollWheelValue;
            }
        }
        public bool LeveTlacitkoStisknuto()
        {
            return mysStaryState.LeftButton == ButtonState.Released && mysState.LeftButton == ButtonState.Pressed;
        }

        public bool LeveTlacitkoDrzeno()
        {
            return mysStaryState.LeftButton == ButtonState.Pressed && mysState.LeftButton == ButtonState.Pressed;
        }

        public bool LeveTlacitkoPusteno()
        {
            return mysStaryState.LeftButton == ButtonState.Pressed && mysState.LeftButton == ButtonState.Released;
        }


        public bool StredniTlacitkoStisknuto()
        {
            return mysStaryState.MiddleButton == ButtonState.Released && mysState.MiddleButton == ButtonState.Pressed;
        }

        public bool StredniTlacitkoDrzeno()
        {
            return mysStaryState.MiddleButton == ButtonState.Pressed && mysState.MiddleButton == ButtonState.Pressed;
        }

        public bool StredniTlacitkoPusteno()
        {
            return mysStaryState.MiddleButton == ButtonState.Pressed && mysState.MiddleButton == ButtonState.Released;
        }


        public bool PraveTlacitkoStisknuto()
        {
            return mysStaryState.RightButton == ButtonState.Released && mysState.RightButton == ButtonState.Pressed;
        }

        public bool PraveTlacitkoDrzeno()
        {
            return mysStaryState.RightButton == ButtonState.Pressed && mysState.RightButton == ButtonState.Pressed;
        }

        public bool PraveTlacitkoPusteno()
        {
            return mysStaryState.RightButton == ButtonState.Pressed && mysState.RightButton == ButtonState.Released;
        }
#endregion

        

    }
}
