class Return1 {
    public static void main(String[] args) throws Exception
    {
        try {
            throw new Exception("Exception");
        } 
        finally {
            System.out.println("finally");
        }
    }
}
