using System;

namespace Algorithm
{
    public abstract class Element
    {
        

        public abstract float CalcValue();

        public abstract bool HasVariable();
    }

    public class Constant : Element
    {
        protected float m_val = 0;

        public Constant(float val = 0)
        {
            m_val = val;
        }

        public override float CalcValue()
        {
            return m_val;
        }

        public override bool HasVariable()
        {
            return false;
        }

        public override string ToString()
        {
            return CalcValue().ToString();
        }
    }

    public class Variable : Element
    {
        protected string m_name = "X";

        public float Val
        {
            get;
            set;
        }

        public string Name
        {
            get { return m_name; }
        }

        public Variable(string name, float defaultVal = 0)
        {
            if (string.IsNullOrWhiteSpace(name)) throw new ArgumentException("name can not be empty!");
            Val = defaultVal;
        }

        public override string ToString()
        {
            return Name;
        }

        public override float CalcValue()
        {
            return Val;
        }

        public override bool HasVariable()
        {
            return true;
        }
    }

    public enum Operate
    {
        Plus,
        Minus,
        Multiply,
        Divide
    }

    public class BinaryOperate : Element
    {
        protected Element m_left;
        protected Element m_right;
        protected Operate m_op = Operate.Multiply;

        public Element Left
        {
            get { return m_left; }
        }

        public Element Right
        {
            get { return m_right; }
        }

        public Operate Operate
        {
            get { return m_op; }
        }

        public BinaryOperate(Element left, Element right, Operate op = Operate.Multiply)
        {
            if (left == null || right == null) throw new ArgumentNullException("element is null!");
            m_left = left;
            m_right = right;
            m_op = op;
        }

        public override string ToString()
        {
            if (!HasVariable()) return CalcValue().ToString();
            else
            {
                string leftStr = Left.ToString(), rightstr = Right.ToString();
                if ((Right is BinaryOperate)&&(Operate == Operate.Multiply || Operate == Operate.Divide))
                        rightstr = string.Format("({0})", rightstr);
 
                switch (Operate)
                {
                    case Operate.Plus:
                        return string.Format("{0}+{1}", leftStr, rightstr);

                    case Operate.Minus:
                        return string.Format("{0}-{1}", leftStr, rightstr);

                    case Operate.Multiply:
                        return string.Format("{0}*{1}", leftStr, rightstr);

                    case Operate.Divide:
                        return string.Format("{0}/{1}", leftStr, rightstr);
                 
                    default:
                        return string.Format("{0}+{1}", leftStr, rightstr);
                }
            }
        }

        public override float CalcValue()
        {
            float left = Left.CalcValue(), right = Right.CalcValue();
            switch (Operate)
            {
                case Operate.Plus:
                    return left + right;

                case Operate.Minus:
                    return left - right;

                case Operate.Multiply:
                    return left * right;

                case Operate.Divide:
                    return left / right;

                default:
                    return left + right;
            }
        }

        public override bool HasVariable()
        {
            return Left.HasVariable() || Right.HasVariable();
        }

    }


     


    public class MatrixElement
    {
        protected Element[] items = new Element[16];
    }
}